#include "TestCore.h"

#include "Examples\TestDraw.h"

#include "Containers\Colour.h"
#include "CoreComponents\DrawableComponents.h"
#include "CoreComponents\LightComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\TextModule.h"

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
#pragma region 3D
		{
			// --- Create the test entity ---
			entt::entity testEntity = m_Context.m_Registry.create();

			// Create a model component
			ecs::ModelComponent modelComponent = {};
			modelComponent.m_MeshPath = "Tests/Data/Rendering/ball_model.obj";
			modelComponent.m_TexturePath = "Tests/Data/Rendering/ball_texture.png";
			modelComponent.m_PixelShaderPath = "Tests/Data/Rendering/PixelShader3D.cso";
			modelComponent.m_VertexShaderPath = "Tests/Data/Rendering/VertexShader3D.cso";

			// Create transform component
			ecs::TransformComponent transformComponent = {};
			transformComponent.m_Position = { 0, 0, 0 };
			transformComponent.m_Rotation = { 0, 0, 0 };
			transformComponent.m_Scale = { 1, 1, 1 };

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

			lightComponent.m_AmbientIntensity = 1.0f;
			lightComponent.m_DiffuseIntensity = 1.0f;
			lightComponent.m_SpecularAttenuation = 1.0f;
			lightComponent.m_SpecularIntensity = 1.0f;

			// Add the light and transform component to the entity
			m_Context.m_Registry.emplace<ecs::DirectionalLightComponent>(lightEntity, lightComponent);
			m_Context.m_Registry.emplace<ecs::TransformComponent>(lightEntity, transformComponent);
		}
#pragma endregion

#pragma region 2D
		{
			// --- Create the test entity ---
			entt::entity testEntity = m_Context.m_Registry.create();

			// Create a sprite component
			ecs::SpriteComponent spriteComponent = {};
			spriteComponent.m_ImagePath = "Tests/Data/Rendering/font.png"; // sprite.png
			spriteComponent.m_PixelShaderPath = "Tests/Data/Rendering/PixelShader2D.cso";
			spriteComponent.m_VertexShaderPath = "Tests/Data/Rendering/VertexShader2D.cso";

			nabi::Rendering::UVs const textUvs = ecs::TextModule::CalculateCharacterUvs('b', 32, { 15, 8 });
			spriteComponent.m_U = { textUvs.m_U1, textUvs.m_U2 };
			spriteComponent.m_V = { textUvs.m_V1, textUvs.m_V2 };

			// then skybox and scene coords! then.. done??

			// rename model, sprite and text component to like modelresourcecomponent, spriteresourcecomponent etc
			// dont use a buffer group component. need a generic spatialheriechy component instead
			// then a text system, or some sort of text helper free functions, can take in a text component (which will efficietly just be a tag)
			// find the child entities (which will basically be sprites - transform, buffercomponent, etc) and update there pos / content accordingly
			// whatever these helper functions are, these should probs be used wherever the textloader creates the text as well for DRY

			// Perhaps have a core resource loader module which can load modules/sprites/text
			
			// Create transform component
			ecs::TransformComponent transformComponent = {};
			transformComponent.m_Position = { -1, 0, 0 };
			transformComponent.m_Rotation = { 0, 0, 0 };
			transformComponent.m_Scale = { 0.5, 0.5, 0.5 };

			// Add the model component and a transform to the entity
			m_Context.m_Registry.emplace<ecs::SpriteComponent>(testEntity, spriteComponent);
			m_Context.m_Registry.emplace<ecs::TransformComponent>(testEntity, transformComponent);
		}
#pragma endregion

		// --- Load all assets ---
		m_AssetBank->LoadAssets();

		return true;
	}

	bool TestDraw::Update()
	{
		m_Context.m_Registry.view<ecs::TransformComponent, ecs::BufferComponent>()
			.each([&](entt::entity const entity, auto& transformComponent, auto const& modelComponent)
				{
					float constexpr speed = 0.001f;
					//transformComponent.m_Rotation.x += speed;
					//transformComponent.m_Rotation.y += speed;
					//transformComponent.m_Rotation.z += speed;
				});

		return true;
	}

	bool TestDraw::Render()
	{
		m_LightingSystem.Render();
		m_RenderSystem.Render3D();
		m_RenderSystem.Render2D();

		return true;
	}

#pragma endregion

#pragma region Asset Bank

	TestDraw::TestAssetBank::TestAssetBank(nabi::Context& context)
		// Base
		: AssetBank(context)

		// Banks
		, m_RenderBufferBank(context)
		, m_PixelShaderBank(context)
		, m_VertexShaderBank(context)
		, m_TextureBank(context)
	{
	}

	TestDraw::TestAssetBank::~TestAssetBank()
	{
		UnloadAssets();
	}

	bool TestDraw::TestAssetBank::LoadAssets()
	{
		bool loadingSuccess = true;
		loadingSuccess &= Load3DModels();
		loadingSuccess &= Load2DSprites();

		return loadingSuccess;
	}

	bool TestDraw::TestAssetBank::UnloadAssets()
	{
		m_RenderBufferBank.Clear();
		m_PixelShaderBank.Clear();
		m_VertexShaderBank.Clear();
		m_TextureBank.Clear();

		return true;
	}

	bool TestDraw::TestAssetBank::Load3DModels()
	{
		// Namespaces for clarity
		using namespace nabi::Rendering;
		using namespace nabi::Resource;

		// Set the banks for meshes
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		vertexShaderLoader.SetInputLayout(Layouts::c_MeshInputLayout);
		vertexShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerFrame, ConstantBufferIndex::PerMesh });

		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();
		pixelShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerLightChange });

		RenderBufferLoader& renderBufferLoader = m_RenderBufferBank.GetLoader();
		renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_3D);

		// Iterate through all the entities with model components
		m_Context.m_Registry.view<ecs::ModelComponent>()
			.each([&](entt::entity const entity, auto const& modelComponent)
				{
					// Mesh
					ResourceRef<Mesh> const meshResource = m_RenderBufferBank.LoadResource(modelComponent.m_MeshPath);

					ecs::BufferComponent meshComponent = {};
					meshComponent.m_BufferResource = meshResource;

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
					m_Context.m_Registry.emplace_or_replace<ecs::BufferComponent>(entity, meshComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(entity, shaderComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(entity, textureComponent);
				});

		return true;
	}

	bool TestDraw::TestAssetBank::Load2DSprites()
	{
		// Set the banks for loading 2D sprites
		using namespace nabi::Rendering;
		using namespace nabi::Resource;
		
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		vertexShaderLoader.SetInputLayout(Layouts::c_SpriteInputLayout);
		vertexShaderLoader.SetConstantBuffers({});

		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();
		pixelShaderLoader.SetConstantBuffers({});

		RenderBufferLoader& renderBufferLoader = m_RenderBufferBank.GetLoader();
		renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_2D);

		// Iterate through all the entities with sprite components
		m_Context.m_Registry.view<ecs::SpriteComponent>()
			.each([&](entt::entity const entity, auto const& spriteComponent)
				{
					// Sprite
					renderBufferLoader.SetSpriteSheetProperties(spriteComponent.m_U, spriteComponent.m_V);
					ResourceRef<Sprite> const spriteResource = m_RenderBufferBank.LoadResource(spriteComponent.m_ImagePath);

					ecs::BufferComponent imageComponent = {};
					imageComponent.m_BufferResource = spriteResource;

					// Shaders
					ResourceRef<PixelShader> const pixelShaderResource = m_PixelShaderBank.LoadResource(spriteComponent.m_PixelShaderPath);
					ResourceRef<VertexShader> const vertexShaderResource = m_VertexShaderBank.LoadResource(spriteComponent.m_VertexShaderPath);

					ecs::ShaderComponent shaderComponent = {};
					shaderComponent.m_PixelShaderResource = pixelShaderResource;
					shaderComponent.m_VertexShaderResource = vertexShaderResource;

					// Texture
					ResourceRef<Texture> const textureResource = m_TextureBank.LoadResource(spriteComponent.m_ImagePath);

					ecs::TextureComponent textureComponent = {};
					textureComponent.m_TextureResource = textureResource;

					// Assign the components to the registery
					m_Context.m_Registry.emplace_or_replace<ecs::BufferComponent>(entity, imageComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(entity, shaderComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(entity, textureComponent);
				});

		return true;
	}

	bool TestDraw::TestAssetBank::LoadText()
	{
		m_Context.m_Registry.view<ecs::TextComponent>()
			.each([&](entt::entity const entity, auto const& textComponent)
				{

				});

		return true;
	}

#pragma endregion
} // namespace nabi::Examples

#endif // #ifdef RUN_TESTS
