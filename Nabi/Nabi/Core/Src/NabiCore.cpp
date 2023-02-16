#include "EngineCore.h"

#include "NabiCore.h"

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
		, m_Ctx{}
	{
		// --- Setup the Context ---
		// Entity
		m_Ctx.m_Registry = {};
		m_Ctx.m_SingletonEntites.fill(entt::null);

		// Rendering
		m_Ctx.m_RenderCommand = std::make_unique<Rendering::RenderCommand>(m_DXObjects, m_Window.GetHWND(), initSettings.m_WindowSettings);
	}

	NabiCore::~NabiCore()
	{
		m_DXObjects.m_Context->ClearState();
	}

	int NabiCore::Init() NABI_NOEXCEPT
	{
		return NABI_SUCCESS;
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
		return m_Ctx;
	}

	void NabiCore::Render() NABI_NOEXCEPT
	{
		m_Ctx.m_RenderCommand->BeginFrame();

		// Render code goes here

		m_Ctx.m_RenderCommand->EndFrame();
	}
} // namespace nabi
