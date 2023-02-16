#pragma once
#include "EngineCore.h"
#include "WinCore.h"

#include "entt.h"

#include "Context.h"
#include "DXObjects.h"
#include "Window.h"

namespace nabi
{
	// Forward Declares
	struct NabiCoreSettings;

	/// <summary>
	/// The base of all things Nabi. This is kinda big deal
	/// </summary>
	class NabiCore 
#ifndef ENGINE_DEVELOPMENT
		abstract
#endif // !ENGINE_DEVELOPMENT
	{
	public:
		NabiCore(HINSTANCE const hInstance, NabiCoreSettings const& initSettings) NABI_NOEXCEPT;
		virtual ~NabiCore();

		[[nodiscard]] virtual int Init() NABI_NOEXCEPT;
		[[nodiscard]] virtual int Run() NABI_NOEXCEPT;

		[[nodiscard]] inline Context const& GetContext() NABI_NOEXCEPT;

	protected:
		//virtual void Update(gametime) NABI_NOEXCEPT;
		virtual void Render(/*gametime?*/) NABI_NOEXCEPT;

		// Windows
		HINSTANCE const m_hInstance;
		Window m_Window;

		// DirectX
		Rendering::DXObjects m_DXObjects;

		// Nabi
		Context m_Ctx;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(NabiCore)
	};
} // namespace nabi
