#include "TestCore.h"

#include "Examples\TestDraw.h"

#include "Containers\Colour.h"
#include "CoreComponents\LightComponent.h"
#include "CoreComponents\ModelComponent.h"
#include "CoreComponents\TransformComponent.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
#pragma region Draw

	TestDraw::TestDraw(nabi::Context& context)
		: m_Context(context)
		, m_RenderSystem(context, "RenderId"_hs, "GroupId"_hs)
		, m_LightingSystem(context, "LightingId"_hs, "GroupId"_hs)
		, m_AssetBank(std::make_unique<TestAssetBank>(context))
	{
	}

	bool TestDraw::Init()
	{
		// --- Create the test entity ---
		entt::entity testEntity = m_Context.m_Registry.create();

		// Create a model component
		ecs::ModelComponent modelComponent = {};
		modelComponent.m_MeshPath = "Tests/Data/Rendering/ball_model.obj";
		modelComponent.m_TexturePath = "Tests/Data/Rendering/ball_texture.png";
		modelComponent.m_PixelShaderPath = "Tests/Data/Rendering/PixelShader.cso";
		modelComponent.m_VertexShaderPath = "Tests/Data/Rendering/VertexShader.cso";

		// Create transform component
		ecs::TransformComponent transformComponent = {};
		transformComponent.m_Position = { 0, 0, 0 };
		transformComponent.m_Rotation = { 0, 0, 0 };
		transformComponent.m_Scale    = { 1, 1, 1 };

		// Add the model component and a transform to the entity
		m_Context.m_Registry.emplace<ecs::ModelComponent>(testEntity, modelComponent);
		m_Context.m_Registry.emplace<ecs::TransformComponent>(testEntity, transformComponent);

		// --- Create a light ---
		entt::entity lightEntity = m_Context.m_Registry.create();

		// Reuse the transform component
		transformComponent.m_Position = { 0, 1, 0 };

		// Create the light
		ecs::DirectionalLightComponent lightComponent;
		lightComponent.m_Colour = static_cast<dx::XMFLOAT3>(nabi::Rendering::Colours::White);
		lightComponent.m_Direction = { -10, 0, -20 };
		lightComponent.m_Intensity = 1.0f;

		// Add the light and transform component to the entity
		m_Context.m_Registry.emplace<ecs::DirectionalLightComponent>(lightEntity, lightComponent);
		m_Context.m_Registry.emplace<ecs::TransformComponent>(lightEntity, transformComponent);

		// --- Load all assets ---
		m_AssetBank->LoadAssets();

		return true;
	}

	bool TestDraw::Update()
	{
		/*
		m_Context.m_Registry.view<ecs::TransformComponent, ecs::DirectionalLightComponent>()
			.each([&](entt::entity const entity, auto& transformComponent, auto& directionalLightComponent)
		{
		});
		*/

		m_Context.m_Registry.view<ecs::TransformComponent, ecs::MeshComponent>()
			.each([&](entt::entity const entity, auto& transformComponent, auto const& modelComponent)
				{
					float constexpr speed = 0.001f;
					transformComponent.m_Rotation.x += speed;
					transformComponent.m_Rotation.y += speed;
					transformComponent.m_Rotation.z += speed;
				});

		return true;
	}

	bool TestDraw::Render()
	{
		m_LightingSystem.Render();
		m_RenderSystem.Render();

		return true;
	}

#pragma endregion

#pragma region Asset Bank

	TestDraw::TestAssetBank::TestAssetBank(nabi::Context& context)
		// Base
		: AssetBank(context)

		// Banks
		, m_MeshBank(context)
		, m_PixelShaderBank(context)
		, m_VertexShaderBank(context)
		, m_TextureBank(context)
	{
		using namespace nabi::Rendering;

		// Need to setup the vertex shader bank loader for meshes
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		vertexShaderLoader.SetInputLayout(Layouts::c_MeshInputLayout);
		vertexShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerFrame, ConstantBufferIndex::PerMesh });

		// And the pixel shader for lighting
		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();
		pixelShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerLightChange, ConstantBufferIndex::PerGlobalLightingChange });
	}

	TestDraw::TestAssetBank::~TestAssetBank()
	{
		UnloadAssets();
	}

	bool TestDraw::TestAssetBank::LoadAssets()
	{
		bool loadingSuccess = true;
		loadingSuccess &= Load3DModels();

		return loadingSuccess;
	}

	bool TestDraw::TestAssetBank::UnloadAssets()
	{
		m_MeshBank.Clear();
		m_PixelShaderBank.Clear();
		m_VertexShaderBank.Clear();
		m_TextureBank.Clear();

		return true;
	}

	bool TestDraw::TestAssetBank::Load3DModels()
	{
		// Iterate through all the entities with model components
		m_Context.m_Registry.view<ecs::ModelComponent>()
			.each([&](entt::entity const entity, auto const& modelComponent)
				{
					// Namespaces for clarity
					using namespace nabi::Rendering;
					using namespace nabi::Resource;

					// Mesh
					ResourceRef<Mesh> const meshResource = m_MeshBank.LoadResource(modelComponent.m_MeshPath);

					ecs::MeshComponent meshComponent = {};
					meshComponent.m_MeshResource = meshResource;

					// Shaders
					ResourceRef<PixelShader> const pixelShaderResource = m_PixelShaderBank.LoadResource(modelComponent.m_PixelShaderPath);
					ResourceRef<VertexShader> const vertexShaderResource = m_VertexShaderBank.LoadResource(modelComponent.m_VertexShaderPath);

					ecs::ShaderComponent shaderComponent = {};
					shaderComponent.m_PixelShaderResource = pixelShaderResource;
					shaderComponent.m_VertexShaderResource = vertexShaderResource;

					// Texture
					ResourceRef<Texture> const textureResource = m_TextureBank.LoadResource(modelComponent.m_TexturePath);

					ecs::TextureComponent textureComponent = {};
					textureComponent.m_TextureResource = textureResource;

					// Assign the components to the registery
					m_Context.m_Registry.emplace_or_replace<ecs::MeshComponent>(entity, meshComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(entity, shaderComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(entity, textureComponent);
				});

		return true;
	}

#pragma endregion
} // namespace nabi::Examples

#endif // #ifdef RUN_TESTS
