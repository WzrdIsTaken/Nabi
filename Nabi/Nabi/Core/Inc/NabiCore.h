#pragma once
#include "EngineCore.h"
#include "WinCore.h"

#include "entt.h"

#include "Context.h"
#include "DXObjects.h"
#include "WindowEventsListener.h"

// TEST
#include "Examples\TestDraw.h"

namespace nabi
{
	// Forward Declares
	struct NabiCoreSettings;

	/// <summary>
	/// The base of all things Nabi. This is kinda big deal
	/// </summary>
	class NabiCore 
#ifndef ENGINE_DEVELOPMENT
		abstract // So we can create a base instance of Nabi in main to easily test core features
#endif // !ENGINE_DEVELOPMENT
	{
	public:
		NabiCore(HINSTANCE const hInstance, NabiCoreSettings const& initSettings) NABI_NOEXCEPT;
		virtual ~NabiCore();

		[[nodiscard]] virtual int Init() NABI_NOEXCEPT;
		[[nodiscard]] virtual int Run() NABI_NOEXCEPT;

		[[nodiscard]] inline Context const& GetContext() NABI_NOEXCEPT;

	protected:
		virtual void Update(/*gametime?*/) /*const?*/ NABI_NOEXCEPT;
		virtual void Render(/*gametime?*/) /*const?*/ NABI_NOEXCEPT;

		bool const InitGraphicsEntity();
		bool const InitDxPipeline();
		bool const InitInputEntity();

		// Windows
		HINSTANCE const m_hInstance;
		WindowEventsListener m_WindowEventsListener;

		// DirectX
		Rendering::DXObjects m_DXObjects;

		// Nabi
		Context m_Context;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(NabiCore)

		nabitest::Examples::TestDraw test_Draw;
	};
} // namespace nabi
