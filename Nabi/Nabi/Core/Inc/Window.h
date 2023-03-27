#pragma once
#include "EngineCore.h"
#include "WinCore.h"

#include "entt.h"

#include "InitSettings.h"

#define REGISTER_WINDOWS_EVENT_SUBSCRIBER(msg, subscriber) \
	{ \
		REGISTER_UNREGISTER_WINDOWS_EVENT_HELPER(msg) \
		sink.connect<&subscriber>(this); \
	}
#define UNREGISTER_WINDOWS_EVENT_SUBSCRIBER(msg, subscriber) \
	{ \
		REGISTER_UNREGISTER_WINDOWS_EVENT_HELPER(msg) \
		sink.disconnect<&subscriber>(this); \
	}
#define REGISTER_UNREGISTER_WINDOWS_EVENT_HELPER(msg) \
	Window::WindowsMsg& sigh = m_Context.m_Window->GetOrAddEvent(msg); \
	entt::sink sink{ sigh };

namespace nabi
{
	class Window final
	{
	public:
		typedef entt::sigh<void(WPARAM, LPARAM)> WindowsMsg; 

		Window(HINSTANCE const hInstance, WindowSettings const& settings) NABI_NOEXCEPT;
		~Window();

		WindowsMsg& GetOrAddEvent(UINT const messageId) NABI_NOEXCEPT;
		bool RemoveEvent(UINT const messageId) NABI_NOEXCEPT;

		/// <summary>
		/// TODO - Note how this func is static because it should process messages for all windows
		/// TODO - const up stuff here and [[nodisacrd]] etc
		/// </summary>
		/// <returns></returns>
		static std::optional<int> ProcessMessages() NABI_NOEXCEPT;
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		HWND const GetHWND() const NABI_NOEXCEPT;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(Window)

		struct WindowsMsgPair;
		typedef std::vector<WindowsMsgPair>::iterator WindowMsgItr;

		struct WindowsMsgPair
		{
			UINT m_Msg;
			WindowsMsg m_Event;
		};

		enum class FindMode : int
		{
			Find,
			Remove,
			ENUM_COUNT
		};

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;
		static LRESULT CALLBACK HandleMsgProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;

		WindowMsgItr FindMsgItr(UINT const messageId, FindMode const findMode) NABI_NOEXCEPT;
		bool IsMsgItrValid(WindowMsgItr const itr) const NABI_NOEXCEPT;

		HWND m_hWnd;
		std::vector<WindowsMsgPair> m_WindowsEvents;

		HINSTANCE const c_hInstance;
		LPCWSTR const c_WindowClassName;
	};
} // namespace nabi::Rendering
