#include "EngineCore.h"

#include "NabiCore.h"

#include "CoreComponents\CameraComponent.h"
#include "CoreModules\CameraModule.h"
#include "CoreSingletonComponents\CollisionStateComponent.h"
#include "CoreSingletonComponents\GraphicsComponent.h"
#include "CoreSingletonComponents\InputStateComponent.h"
#include "CoreSingletonComponents\UIStateComponent.h"
#include "CoreSingletonComponents\LightStateComponent.h"
#include "EntityCreator.h"
#include "MetaObjectLookup.h"
#include "RenderCommand.h"
#include "XmlParser.h"

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
		, m_GameTime{}
		, m_InitSettings(initSettings)

		// TEST
		, test_Input(m_Context)
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
		bool initializationSuccessful = true;

		// Init core entities
		initializationSuccessful &= InitGraphicsEntity();
		initializationSuccessful &= InitDxPipeline();
		initializationSuccessful &= InitInputEntity();
		initializationSuccessful &= InitPhysicsEntity();

		// Parse xml
		initializationSuccessful &= ParseECSData();

		// TEST
		test_Input.Init();

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

			m_GameTime.Tick();

			Update();
			Render();
		}

		// If we get to here, somethings gone wrong
		return NABI_FAIL;
	}

	void NabiCore::Update() NABI_NOEXCEPT
	{
#ifdef USE_EVENT_SYSTEM_UPDATE
		m_Context.m_NabiEventsManager.FireSystemUpdateEvent(m_GameTime);
#endif // ifdef USE_META_SYSTEM_UPDATE

		// TEST
		test_Input.Update();
	}

	void NabiCore::Render() NABI_NOEXCEPT
	{
		m_Context.m_RenderCommand->BeginFrame();

#ifdef USE_EVENT_SYSTEM_UPDATE
		m_Context.m_NabiEventsManager.FireSystemRenderEvent(m_GameTime);
#endif // ifdef USE_META_SYSTEM_UPDATE

		// TEST
		test_Input.Render();

		m_Context.m_RenderCommand->EndFrame();
	}

	bool const NabiCore::InitGraphicsEntity() NABI_NOEXCEPT
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
		ecs::CameraModule::DefaultCameraValues(m_Context, perspectiveCamera, defaultCameraSettings);
		ecs::CameraModule::DefaultCameraValues(m_Context, orthographicCamera, defaultCameraSettings);

		// --- Create the graphics component ---
		ecs::SComp::GraphicsComponent graphicsComponent;

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
		ecs::SComp::LightStateComponent lightStateComponent;
		lightStateComponent.m_LightCount = 0u;
		lightStateComponent.m_UpdateLights = false;

		// --- Add the graphics components to the entity ---
		m_Context.m_Registry.emplace<ecs::CameraGroupComponent>(graphicsEntity, cameraComponent);
		m_Context.m_Registry.emplace<ecs::SComp::GraphicsComponent>(graphicsEntity, graphicsComponent);
		m_Context.m_Registry.emplace<ecs::SComp::LightStateComponent>(graphicsEntity, lightStateComponent);

		return true;
	}

	bool const NabiCore::InitDxPipeline() NABI_NOEXCEPT
	{
		using namespace nabi::Rendering;

		// Just create any other DirectX pipeline things here

		// --- Create the sampler ---
		Sampler sampler = m_Context.m_RenderCommand->CreateSampler();
		m_Context.m_RenderCommand->BindSampler(sampler);

		return true;
	}

	bool const NabiCore::InitInputEntity() NABI_NOEXCEPT
	{
		// Create the input entity
		entt::entity const inputEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Input) = m_Context.m_Registry.create();

		// Add the input state component (tracks the state of keys/buttons)
		m_Context.m_Registry.emplace<ecs::SComp::InputStateComponent>(inputEntity);

		// Add the ui state component (tracks the hierachy of ui scenes)
		m_Context.m_Registry.emplace<ecs::SComp::UIStateComponent>(inputEntity);

		// Add the ui storage component (can be used to store data between ui scenes / function calls (as ui scenes are just free functions + wouldn't always exist anyway))
		m_Context.m_Registry.emplace<ecs::SComp::UIStorageComponent>(inputEntity);

		return true;
	}

	bool const NabiCore::InitPhysicsEntity() NABI_NOEXCEPT
	{
		// Create the physics entity
		entt::entity const physicsEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Physics) = m_Context.m_Registry.create();

		// Add the CollisionStateComponent (keeps track of colliders)
		m_Context.m_Registry.emplace<ecs::SComp::CollisionStateComponent>(physicsEntity);

		return true;
	}

	bool const NabiCore::ParseECSData() NABI_NOEXCEPT
	{
		typedef DataSettings::NabiCoreParseMode ParseMode;
		ParseMode const parseMode = m_InitSettings.m_DataSettings.m_NabiCoreParseDocuments;

		if (parseMode != ParseMode::None)
		{
			Reflection::XmlParser xmlParser;
			Reflection::MetaObjectLookup systemsLookup;
			std::string const& routeDoc = m_InitSettings.m_DataSettings.m_RouteDocument;

			switch (parseMode)
			{
			case ParseMode::All:
				xmlParser.ParseXml(routeDoc, m_Context, &systemsLookup);
				break;
			case ParseMode::Systems:
				ASSERT_FAIL("Only parsing systems is currently unhandled by NabiCore, though the functionality exists in XmlParser");
				break;
			case ParseMode::Components:
				ASSERT_FAIL("Only parsing components is currently unhandled by NabiCore, though the functionality exists in XmlParser");
				break;
			default:
				ASSERT_FAIL("Using an unexpected DataSettings::NabiCoreParseMode!");

				// (examples of parsing both systems and components separately can be found in RefectionTests/)
			}

#ifdef USE_EVENT_SYSTEM_UPDATE
			m_Systems = std::move(systemsLookup.m_MetaObjectLookup);
#else
			ASSERT(parseMode == ParseMode::All || parseMode == ParseMode::Systems,
				"Using NabiCore's parse xml functionality but not system event updating is not defined. Systems will fall out of scope, and will not update.");
#endif // USE_EVENT_SYSTEM_UPDATE
		}

		return true;
	}
} // namespace nabi
