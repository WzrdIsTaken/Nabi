#pragma once
#include "EngineCore.h"
#include "WinCore.h"

#include "entt.h"

#include "Context.h"
#include "DXObjects.h"
#include "InitSettings.h"
#include "WindowEventsListener.h"

// TEST
#include "Examples\TestDraw.h"
#include "Examples\TestInput.h"
#include "Examples\TestPhysics.h"

namespace nabi
{
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

		[[nodiscard]] virtual int Init();
		[[nodiscard]] virtual int Run();

		[[nodiscard]] inline Context& GetContext() NABI_NOEXCEPT { return m_Context; }

	protected:
		virtual void Update(/*gametime?*/) /*const?*/ NABI_NOEXCEPT;
		virtual void Render(/*gametime?*/) /*const?*/ NABI_NOEXCEPT;

		bool const InitGraphicsEntity() NABI_NOEXCEPT;
		bool const InitDxPipeline() NABI_NOEXCEPT;
		bool const InitInputEntity() NABI_NOEXCEPT;

		bool const ParseECSData() NABI_NOEXCEPT; // The user defined xml ecs data

		// Windows
		HINSTANCE const m_hInstance;
		WindowEventsListener m_WindowEventsListener;

		// DirectX
		Rendering::DXObjects m_DXObjects;

		// Nabi
		Context m_Context;
#ifdef USE_EVENT_SYSTEM_UPDATE
		std::unordered_map<std::string, entt::meta_any> m_Systems;
#endif // ifdef USE_EVENT_SYSTEM_UPDATE
		NabiCoreSettings m_InitSettings;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(NabiCore)

		nabitest::Examples::TestPhysics test_Input;
	};
} // namespace nabi
