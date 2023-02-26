#include "TestCore.h"

#include "Examples\TestDraw.h"

#include "CoreComponents\ModelComponent.h"
#include "CoreComponents\TransformComponent.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
#pragma region Draw

	TestDraw::TestDraw(nabi::Context& context)
		: m_Context(context)
		, m_RenderSystem(context, "Id"_hs, "GroupId"_hs)
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

		// --- Load all assets ---
		m_AssetBank->LoadAssets();

		return true;
	}

	bool TestDraw::Update()
	{
		return false;
	}

	bool TestDraw::Render()
	{
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
		// Need to setup the vertex shader bank loader for meshes
		nabi::Rendering::VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		vertexShaderLoader.SetInputLayout(nabi::Rendering::Layouts::c_MeshInputLayout);
		vertexShaderLoader.SetConstantBuffers({ nabi::Rendering::ConstantBufferIndex::PerFrame, nabi::Rendering::ConstantBufferIndex::PerMesh });

		// TODO - Check over this^ / stuff you can see on github desktop changelist
		// Use ConstantBufferLoader::AssignShaderConstnantBuffers to add the constant buffers to the vertex shader

		// need a camera system
		// the stuff in nabi core setting up the camera should go in there
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
