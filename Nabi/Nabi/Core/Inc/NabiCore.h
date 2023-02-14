#pragma once
#include "EngineCore.h"
#include "WinCore.h"

#include "entt.h"

#include "Context.h"
#include "InitSettings.h"
#include "Window.h"

namespace nabi
{
	/// <summary>
	/// The base of all things Nabi. This is kinda big deal
	/// </summary>
	class NabiCore 
#ifndef ENGINE_DEVELOPMENT
		abstract
#endif // !ENGINE_DEVELOPMENT
	{
	public:
		NabiCore(HINSTANCE hInstance, NabiCoreSettings const& initSettings) NABI_NOEXCEPT;
		virtual ~NabiCore();

		virtual int Init() NABI_NOEXCEPT;
		virtual int Run() NABI_NOEXCEPT;

		[[nodiscard]] inline entt::registry& GetRegistry() NABI_NOEXCEPT;

	protected:
		//virtual void Update(gametime) NABI_NOEXCEPT;
		//virtual void Render(gametime) NABI_NOEXCEPT;

		// Windows Core
		HINSTANCE m_hInstance; // const?
		Window m_Window;

		// Nabi Core
		Context m_Ctx;

		entt::registry m_Registry;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(NabiCore)
	};
} // namespace nabi
