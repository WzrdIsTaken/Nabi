#include "EngineCore.h"

#include "NabiCore.h"

namespace nabi
{
	NabiCore::NabiCore(HINSTANCE hInstance, NabiCoreSettings const& initSettings) NABI_NOEXCEPT
		// Windows
		: m_hInstance(hInstance)
		, m_Window(m_hInstance, initSettings.m_WindowSettings)
	{
	}

	NabiCore::~NabiCore()
	{
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
			// Render();
		}

		// If we get to here, somethings gone wrong
		return NABI_FAIL;
	}

	inline entt::registry& NabiCore::GetRegistry() NABI_NOEXCEPT
	{
		return m_Registry;
	}
} // namespace nabi
