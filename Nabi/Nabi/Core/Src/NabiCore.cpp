#include "EngineCore.h"

#include "NabiCore.h"

#include "CoreComponents\CameraComponent.h"
#include "CoreComponents\GraphicsComponent.h"
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

			// Update();
			Render();
		}

		// If we get to here, somethings gone wrong
		return NABI_FAIL;
	}

	inline Context const& NabiCore::GetContext() NABI_NOEXCEPT
	{
		return m_Context;
	}

	void NabiCore::Render() NABI_NOEXCEPT
	{
		m_Context.m_RenderCommand->BeginFrame();

		// Render code goes here

		// TEST
		test_Draw.Render();

		m_Context.m_RenderCommand->EndFrame();
	}

	bool NabiCore::InitGraphicsEntity()
	{
		using namespace nabi::Rendering;

		// --- Create the graphics entity ---
		entt::entity const graphicsEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Graphic) = m_Context.m_Registry.create();

		// --- Create the camera ---
		ecs::CameraComponent cameraComponent;

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

	bool NabiCore::InitInputEntity()
	{
		// Create the input entity
		entt::entity const inputEntity =
			m_Context.m_SingletonEntites.at(Context::SingletonEntities::Input) = m_Context.m_Registry.create();

		return true;
	}
} // namespace nabi
