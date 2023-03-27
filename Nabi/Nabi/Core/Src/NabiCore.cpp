#include "EngineCore.h"

#include "NabiCore.h"

#include "CoreComponents\CameraComponent.h"
#include "CoreModules\CameraModule.h"
#include "CoreSingletonComponents\GraphicsComponent.h"
#include "CoreSingletonComponents\LightStateComponent.h"
#include "EntityCreator.h"
#include "InitSettings.h"
#include "RenderCommand.h"

namespace nabi
{
	NabiCore::NabiCore(HINSTANCE const hInstance, NabiCoreSettings const& initSettings) NABI_NOEXCEPT
		// Windows
		: m_hInstance(hInstance)
		, m_WindowEventsListener(m_Context, initSettings.m_WindowSettings)

		// Direct X
		, m_DXObjects(Rendering::dxObjectsDefaultSettings)

		// Nabi
		, m_Context{}

		// TEST
		, test_Draw(m_Context)
	{
		// --- Setup the Context ---
		// Core
		m_Context.m_Window = std::make_unique<Window>(m_hInstance, initSettings.m_WindowSettings);

		// Entity
		m_Context.m_Registry = {};
		m_Context.m_SingletonEntites.fill(entt::null);

		m_Context.m_EntityCreator = std::make_unique<Reflection::EntityCreator>(m_Context.m_Registry);

		// Rendering
		m_Context.m_RenderCommand = std::make_unique<Rendering::RenderCommand>(m_DXObjects, m_Context.m_Window->GetHWND(), initSettings.m_WindowSettings);

		// --- Setup windows events ---
		m_WindowEventsListener.RegisterEvents();
	}

	NabiCore::~NabiCore()
	{
		m_Context.m_Registry.clear();
		m_DXObjects.m_Context->ClearState();
		m_WindowEventsListener.UnRegisterEvents();
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
		ecs::CameraGroupComponent cameraComponent = {};

		ecs::CameraModule::DefaultCameraValuesSettings defaultCameraSettings;
		defaultCameraSettings.m_WindowWidth  = static_cast<float>(m_DXObjects.m_Viewport.Width);
		defaultCameraSettings.m_WindowHeight = static_cast<float>(m_DXObjects.m_Viewport.Height);

		ecs::CameraComponent& perspectiveCamera = cameraComponent.m_Cameras.at(ecs::CameraIndex::Perspective);
		ecs::CameraComponent& orthographicCamera = cameraComponent.m_Cameras.at(ecs::CameraIndex::Orthographic);
		ecs::CameraModule::DefaultCameraValues(perspectiveCamera, defaultCameraSettings);
		ecs::CameraModule::DefaultCameraValues(orthographicCamera, defaultCameraSettings);

		// --- Create the graphics component ---
		ecs::GraphicsComponent graphicsComponent;

		// Create the constant buffers
		ConstantBufferLoader constantBufferLoader;

		ConstantBuffer const perFrameConstantBuffer = constantBufferLoader(sizeof(PerFrame), m_Context);
		ConstantBuffer const perMeshConstantBuffer = constantBufferLoader(sizeof(PerRenderable), m_Context);
		ConstantBuffer const perLightChangeConstantBuffer = constantBufferLoader(sizeof(PerLightChange), m_Context);

		// Assign the constant buffers to the component
		graphicsComponent.m_ConstantBuffers.at(ConstantBufferIndex::Enum::PerFrame) = perFrameConstantBuffer;
		graphicsComponent.m_ConstantBuffers.at(ConstantBufferIndex::Enum::PerRenderable) = perMeshConstantBuffer;
		graphicsComponent.m_ConstantBuffers.at(ConstantBufferIndex::Enum::PerLightChange) = perLightChangeConstantBuffer;

		// --- Create the light state component ---
		ecs::LightStateComponent lightStateComponent;
		lightStateComponent.m_LightCount = 1; // TEST for testing only. should be '0'
		lightStateComponent.m_UpdateLights = true; // TEST for testing only. should be 'false'

		// --- Add the graphics components to the entity ---
		m_Context.m_Registry.emplace<ecs::CameraGroupComponent>(graphicsEntity, cameraComponent);
		m_Context.m_Registry.emplace<ecs::GraphicsComponent>(graphicsEntity, graphicsComponent);
		m_Context.m_Registry.emplace<ecs::LightStateComponent>(graphicsEntity, lightStateComponent);

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
