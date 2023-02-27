#include "EngineCore.h"

#include "NabiCore.h"

#include "CoreComponents\CameraComponent.h"
#include "CoreComponents\GraphicsComponent.h"
#include "EntityCreator.h"
#include "InitSettings.h"
#include "RenderCommand.h"

namespace nabi
{
	NabiCore::NabiCore(HINSTANCE const hInstance, NabiCoreSettings const& initSettings) NABI_NOEXCEPT
		// Windows
		: m_hInstance(hInstance)
		, m_Window(m_hInstance, initSettings.m_WindowSettings)

		// Direct X
		, m_DXObjects(Rendering::dxObjectsDefaultSettings)

		// Nabi
		, m_Context{}

		// TEST
		, test_Draw(m_Context)
	{
		// --- Setup the Context ---
		// Entity
		m_Context.m_Registry = {};
		m_Context.m_SingletonEntites.fill(entt::null);

		m_Context.m_EntityCreator = std::make_unique<Reflection::EntityCreator>(m_Context.m_Registry);

		// Rendering
		m_Context.m_RenderCommand = std::make_unique<Rendering::RenderCommand>(m_DXObjects, m_Window.GetHWND(), initSettings.m_WindowSettings);
	}

	NabiCore::~NabiCore()
	{
		m_DXObjects.m_Context->ClearState();
	}

	int NabiCore::Init() NABI_NOEXCEPT
	{
		// Init core entities
		bool initializationSuccessful = true;
		initializationSuccessful &= InitGraphicsEntity();
		initializationSuccessful &= InitDxPipeline();
		initializationSuccessful &= InitInputEntity();

		// TEST
		test_Draw.Init();

		// Return result
		return initializationSuccessful ? NABI_SUCCESS : NABI_FAIL;
	}

	int NabiCore::Run() NABI_NOEXCEPT
	{
		static bool runGame = true;
		while (runGame)
		{
			if (std::optional<int> const errorCode = Window::ProcessMessages())
			{
				// If the optional has a value, it means we received a WP_QUIT message
				return errorCode.value();
			}

			Update();
			Render();
		}

		// If we get to here, somethings gone wrong
		return NABI_FAIL;
	}

	inline Context const& NabiCore::GetContext() NABI_NOEXCEPT
	{
		return m_Context;
	}

	void NabiCore::Update() NABI_NOEXCEPT
	{
		test_Draw.Update();
	}

	void NabiCore::Render() NABI_NOEXCEPT
	{
		m_Context.m_RenderCommand->BeginFrame();

		// Render code goes here

		// TEST
		test_Draw.Render();

		m_Context.m_RenderCommand->EndFrame();
	}

	bool const NabiCore::InitGraphicsEntity()
	{
		using namespace nabi::Rendering;

		// --- Create the graphics entity ---
		entt::entity const graphicsEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Graphic) = m_Context.m_Registry.create();

		// --- Create the camera ---
		ecs::CameraComponent cameraComponent;

		// TEST BEGIN
		dx::XMMATRIX projMat = dx::XMMatrixPerspectiveFovLH(dx::XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);
		dx::XMMATRIX viewMat = dx::XMMatrixTranslation(0, 0, 6);

		dx::XMStoreFloat4x4(&cameraComponent.m_Projection, projMat);
		dx::XMStoreFloat4x4(&cameraComponent.m_View, viewMat);

		cameraComponent.m_Position = { 0, 0, 0 };
		cameraComponent.m_Target   = { 0, 0, 1 };
		// TEST END

		// --- Create the graphics component ---
		ecs::GraphicsComponent graphicsComponent;

		// Create the constant buffers
		ConstantBufferLoader constantBufferLoader;

		ConstantBuffer const perFrameConstantBuffer = constantBufferLoader(sizeof(PerFrame), m_Context);
		ConstantBuffer const perMeshConstantBuffer = constantBufferLoader(sizeof(PerMesh), m_Context);

		// Assign the constant buffers to the component
		graphicsComponent.m_ConstantBuffers.at(ConstantBufferIndex::Enum::PerFrame) = perFrameConstantBuffer;
		graphicsComponent.m_ConstantBuffers.at(ConstantBufferIndex::Enum::PerMesh) = perMeshConstantBuffer;

		// --- Add the graphics components to the entity ---
		m_Context.m_Registry.emplace<ecs::CameraComponent>(graphicsEntity, cameraComponent);
		m_Context.m_Registry.emplace<ecs::GraphicsComponent>(graphicsEntity, graphicsComponent);

		return true;
	}

	bool const NabiCore::InitDxPipeline()
	{
		using namespace nabi::Rendering;

		// Just create any other DirectX pipeline things here

		// --- Create the sampler ---
		Sampler sampler = m_Context.m_RenderCommand->CreateSampler();
		m_Context.m_RenderCommand->BindSampler(sampler);

		return true;
	}

	bool const NabiCore::InitInputEntity()
	{
		// Create the input entity
		entt::entity const inputEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Input) = m_Context.m_Registry.create();

		return true;
	}
} // namespace nabi
