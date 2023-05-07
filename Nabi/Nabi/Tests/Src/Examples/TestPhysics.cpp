#include "TestCore.h"

#include "Examples\TestPhysics.h"

#include "CoreComponents\RigidbodyComponent.h"
#include "CoreModules\InputModule.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	// --- Physics ---

	TestPhysics::TestPhysics(nabi::Context& context)
		: m_Context(context)
		, m_PlayerEntity(entt::null)
	{
	}

	bool TestPhysics::Init()
	{
		// --- Systems ---

		// Having systems as ptrs gets around init order problems
		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "Input"_hs, "NabiPhysicsTestSystems"_hs);
		m_PhysicsSystem = std::make_unique<ecs::PhysicsSystem>(m_Context, "Physics"_hs, "NabiPhysicsTestSystems"_hs);
		m_RenderSystem = std::make_unique<ecs::RenderSystem>(m_Context, "Render"_hs, "NabiPhysicsTestSystems"_hs);

		// --- Entities ---
		m_PlayerEntity = m_Context.m_EntityCreator->CreateEntity();

		// Core
		ecs::TransformComponent transformComponent = {};
		transformComponent.m_Position = { 0, 0, 0 }; 
		transformComponent.m_Rotation = { 0, 0, 0 };
		transformComponent.m_Scale = { 0.25f, 0.25f, 0.25f };

		// Rendering
		ecs::ModelResourceComponent modelComponent = {};
		modelComponent.m_MeshPath = "PrimativeCube=1x1x1";
		modelComponent.m_TexturePath = "Tests/Data/Rendering/skybox_daybreak.png";
		modelComponent.m_PixelShaderPath = "Tests/Data/Rendering/PixelShaderSkybox.cso";
		modelComponent.m_VertexShaderPath = "Tests/Data/Rendering/VertexShaderSkybox.cso";

		// Physics
		ecs::RigidbodyComponent rigidbodyComponent = {};
		rigidbodyComponent.m_GravityScale = 0.0f;

		// Add everything!
		m_Context.m_Registry.emplace<ecs::TransformComponent>(m_PlayerEntity, transformComponent);
		m_Context.m_Registry.emplace<ecs::ModelResourceComponent>(m_PlayerEntity, modelComponent);
		m_Context.m_Registry.emplace<ecs::RigidbodyComponent>(m_PlayerEntity, rigidbodyComponent);

		// --- Assets ---
		m_AssetBank = std::make_unique<SimpleAssetBank>(m_Context);
		m_AssetBank->LoadAssets();

		return true;
	}

	bool TestPhysics::Update()
	{
		using namespace ecs::InputModule;
		using namespace nabi::Input;

		ecs::RigidbodyComponent& playerEntityRigidbody = m_Context.m_Registry.get<ecs::RigidbodyComponent>(m_PlayerEntity);
		float constexpr testEntitySpeed = 0.1f;
		float constexpr testEntityRotation = 5.0f;

		InputState const resetKeyState = GetKeyboardKey(m_Context, InputCode::Key_Z);

		InputState const wKeyState = GetKeyboardKey(m_Context, InputCode::Key_W);
		InputState const sKeyState = GetKeyboardKey(m_Context, InputCode::Key_S);
		InputState const aKeyState = GetKeyboardKey(m_Context, InputCode::Key_A);
		InputState const dKeyState = GetKeyboardKey(m_Context, InputCode::Key_D);
		InputState const qKeyState = GetKeyboardKey(m_Context, InputCode::Key_Q);
		InputState const eKeyState = GetKeyboardKey(m_Context, InputCode::Key_E);

		
		if (resetKeyState == InputState::Pressed)
		{
			ecs::TransformComponent& playerEntityTransformComponent = m_Context.m_Registry.get<ecs::TransformComponent>(m_PlayerEntity);
			dx::XMFLOAT3 constexpr resetValues = { 0.0f, 0.0f, 0.0f };

			playerEntityRigidbody.m_Velocity = resetValues;
			playerEntityRigidbody.m_AngularVelocity = resetValues;

			playerEntityTransformComponent.m_Position = resetValues;
			playerEntityTransformComponent.m_Rotation = resetValues;
		}

		if (wKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.y += testEntitySpeed;
			playerEntityRigidbody.m_AngularVelocity.x += testEntityRotation;
		}
		if (sKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.y -= testEntitySpeed;
			playerEntityRigidbody.m_AngularVelocity.x -= testEntityRotation;
		}
		if (aKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.x -= testEntitySpeed;
			playerEntityRigidbody.m_AngularVelocity.y += testEntityRotation;
		}
		if (dKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.x += testEntitySpeed;
			playerEntityRigidbody.m_AngularVelocity.y -= testEntityRotation;
		}
		if (qKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.z -= testEntitySpeed;
			playerEntityRigidbody.m_AngularVelocity.x -= testEntityRotation;
		}
		if (eKeyState == InputState::Held)
		{
			playerEntityRigidbody.m_Velocity.z += testEntitySpeed;
			playerEntityRigidbody.m_AngularVelocity.x += testEntityRotation;
		}

		return true;
	}

	bool TestPhysics::Render()
	{
		return false;
	}

	// --- Assets ---

	TestPhysics::SimpleAssetBank::SimpleAssetBank(nabi::Context& context)
		// Base
		: AssetBank(context)

		// Banks
		, m_RenderBufferBank(context)
		, m_PixelShaderBank(context)
		, m_VertexShaderBank(context)
		, m_TextureBank(context)
	{
	}

	TestPhysics::SimpleAssetBank::~SimpleAssetBank()
	{
		UnloadAssets();
	}

	bool TestPhysics::SimpleAssetBank::LoadAssets()
	{
		bool loadingSuccess = true;
		loadingSuccess &= Load3DModels();

		return loadingSuccess;
	}

	bool TestPhysics::SimpleAssetBank::UnloadAssets()
	{
		m_RenderBufferBank.Clear();
		m_PixelShaderBank.Clear();
		m_VertexShaderBank.Clear();
		m_TextureBank.Clear();

		return true;
	}

	bool TestPhysics::SimpleAssetBank::Load3DModels()
	{
		// (just took this straight from TestDraw)

		using namespace nabi::Rendering;
		using namespace nabi::Resource;

		// Set the banks for meshes
		VertexShaderLoader& vertexShaderLoader = m_VertexShaderBank.GetLoader();
		vertexShaderLoader.SetInputLayout(Layouts::c_MeshInputLayout);
		vertexShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerFrame, ConstantBufferIndex::PerRenderable });

		PixelShaderLoader& pixelShaderLoader = m_PixelShaderBank.GetLoader();
		pixelShaderLoader.SetConstantBuffers({ ConstantBufferIndex::PerLightChange });

		RenderBufferLoader& renderBufferLoader = m_RenderBufferBank.GetLoader();
		renderBufferLoader.SetLoadMode(RenderBufferLoader::LoadMode::_3D);

		// Iterate through all the entities with model components
		m_Context.m_Registry.view<ecs::ModelResourceComponent>()
			.each([&](entt::entity const entity, auto const& modelResourceComponent)
				{
					// Mesh
					ResourceRef<Mesh> const meshResource = m_RenderBufferBank.LoadResource(modelResourceComponent.m_MeshPath);

					ecs::BufferComponent meshComponent = {};
					meshComponent.m_BufferResource = meshResource;

					// Shaders
					ResourceRef<PixelShader> const pixelShaderResource = m_PixelShaderBank.LoadResource(modelResourceComponent.m_PixelShaderPath);
					ResourceRef<VertexShader> const vertexShaderResource = m_VertexShaderBank.LoadResource(modelResourceComponent.m_VertexShaderPath);

					ecs::ShaderComponent shaderComponent = {};
					shaderComponent.m_PixelShaderResource = pixelShaderResource;
					shaderComponent.m_VertexShaderResource = vertexShaderResource;

					// Texture
					ResourceRef<Texture> const textureResource = m_TextureBank.LoadResource(modelResourceComponent.m_TexturePath);

					ecs::TextureComponent textureComponent = {};
					textureComponent.m_TextureResource = textureResource;

					// Assign the components to the registery
					m_Context.m_Registry.emplace_or_replace<ecs::BufferComponent>(entity, meshComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::ShaderComponent>(entity, shaderComponent);
					m_Context.m_Registry.emplace_or_replace<ecs::TextureComponent>(entity, textureComponent);

					// Tag
					m_Context.m_Registry.emplace_or_replace<ecs::Tags::DrawPerspective>(entity);
				});

		return true;
	}

} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
