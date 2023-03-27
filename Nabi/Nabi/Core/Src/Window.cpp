#include "EngineCore.h"

#include "Window.h"

#include "DebugUtils.h"

namespace nabi
{
	Window::Window(HINSTANCE const hInstance, WindowSettings const& settings) NABI_NOEXCEPT
		: m_hWnd(nullptr)
		, m_WindowsEvents{}
		, c_hInstance(hInstance)
		, c_WindowClassName(settings.m_ClassName)
	{
		// Create the window class
		WNDCLASSEX window = { NULL };
		window.cbSize = sizeof(window);
		window.style = CS_OWNDC;
		window.lpfnWndProc = HandleMsgSetup;
		window.cbClsExtra = 0;
		window.cbWndExtra = 0;
		window.hInstance = hInstance;
		//window.hIcon = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 128, 128, 0)); TODO - add the cool icon
		//window.hIconSm = static_cast<HICON>(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0)); https://youtu.be/QYGLXhulvVQ?list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD&t=1098
		window.hCursor = nullptr;
		window.hbrBackground = nullptr;
		window.lpszMenuName = nullptr;
		window.lpszClassName = c_WindowClassName;

		// Register the window instance
		DX_ASSERT(RegisterClassEx(&window));

		// Work out the size of the window
		RECT windowRect{ 0, 0, settings.m_Width, settings.m_Height };
		DX_ASSERT(AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE));

		// Create the window style
		LONG windowStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		if (settings.m_AllowResizing)
		{
			windowStyle |= WS_THICKFRAME | WS_MAXIMIZEBOX;
		}

		m_hWnd = CreateWindow(
			c_WindowClassName,     // Window class
			settings.m_WindowName, // Window text
			windowStyle,           // Window style

			// Size and position
			CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,

			nullptr,   // Parent window 
			nullptr,   // Menu
			hInstance, // Instance handle
			this       // Additional application data
		);
		DX_ASSERT(m_hWnd);

		// Finally, show the window!
		DX_ASSERT(ShowWindow(m_hWnd, SW_SHOW));

		// Reserve some space for window's events
		m_WindowsEvents.reserve(settings.m_StartingEventsSize);
	}

	Window::~Window()
	{
		DX_ASSERT(DestroyWindow(m_hWnd));
		m_WindowsEvents.clear();

		DX_ASSERT(UnregisterClass(c_WindowClassName, c_hInstance));
	}

	Window::WindowsMessage& Window::GetOrAddEvent(UINT const messageId) NABI_NOEXCEPT
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_WINDOWS << "Registering a new windows event listener with message id " << messageId << ENDLINE);

		if (auto const messageEvent = FindMsgItr(messageId, FindMode::Find); IsMsgItrValid(messageEvent))
		{
			return messageEvent->m_Event;
		}
		else
		{
			WindowsMessagePair newMessagePair;
			newMessagePair.m_Msg = messageId;
			newMessagePair.m_Event = {};

			WindowsMessagePair& messagePair = m_WindowsEvents.emplace_back(newMessagePair);
			return messagePair.m_Event;
		}
	}

	bool Window::RemoveEvent(UINT const messageId) NABI_NOEXCEPT
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_WINDOWS << "Unregistering a windows event listener with message id " << messageId << ENDLINE);

		if (auto const messageEvent = FindMsgItr(messageId, FindMode::Remove); IsMsgItrValid(messageEvent))
		{
			m_WindowsEvents.erase(messageEvent);
			return true;
		}
		else
		{
			ASSERT_FAIL("Trying to unregister a windows event which doesn't exist! Id " << messageId);
			return false;
		}
	}

	std::optional<int> Window::ProcessMessages() NABI_NOEXCEPT
	{
		MSG msg = { 0 };

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return msg.wParam;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return {};
	}

	HWND const Window::GetHWND() const NABI_NOEXCEPT
	{
		return m_hWnd;
	}

	LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT
	{
		// If the message is about handling window setup, handle that separately
		if (msg == WM_NCCREATE)
		{
			// Extract the window ptr from the creation data
			CREATESTRUCTW const* const creationData = reinterpret_cast<CREATESTRUCTW*>(lParam);
			Window* const window = static_cast<Window*>(creationData->lpCreateParams);

			// Set the WinAPI-managed user data to store a ptr to the window class
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
			// Set the message proc to normal (non-setup) handler now that setup has finished
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgProxy));

			// Forward the message to the class handler
			return window->HandleMsg(hWnd, msg, wParam, lParam);
		}

		// Else if we get a message before WM_NCCREATE, handle it with the default handler
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	LRESULT CALLBACK Window::HandleMsgProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT
	{
		// Get the window ptr
		Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		// Forward it to the HandleMsg function
		return window->HandleMsg(hWnd, msg, wParam, lParam);
	}

	LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT
	{
		// Notify subscribers of any messages
		if (auto const messageEvent = FindMsgItr(msg, FindMode::Find); IsMsgItrValid(messageEvent))
		{
			LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_WINDOWS << "Publishing a message with id " << msg << 
				". wParam: " << wParam << " lParam: " << lParam << ENDLINE);

			messageEvent->m_Event.publish(wParam, lParam);
		}
		
		// If the message is WM_CLOSE, quit
		if (msg == WM_CLOSE)
		{
			// Return early, as we are already handling the destruction of the window - no default behaviour needed
			PostQuitMessage(NABI_SUCCESS);
			return NABI_SUCCESS;
		}

		// Else, return the default handling for windows messages
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	Window::WindowMsgItr Window::FindMsgItr(UINT const messageId, FindMode const findMode) NABI_NOEXCEPT
	{
		// this function could be entry in simon's book worthy

		auto const find = [messageId](WindowsMessagePair const& messagePair) -> bool { return messageId == messagePair.m_Msg; };
		auto const messageSubscribersItr = findMode == FindMode::Find ? std::find_if  (m_WindowsEvents.begin(), m_WindowsEvents.end(), find)
												        /* Remove  */ : std::remove_if(m_WindowsEvents.begin(), m_WindowsEvents.end(), find);
									                    /* Default */   ASSERT(findMode == FindMode::Find || findMode == FindMode::Remove, "yeah dont do that pls tyty");
		return messageSubscribersItr;
	}

	bool Window::IsMsgItrValid(WindowMsgItr const itr) const NABI_NOEXCEPT
	{
		return itr != m_WindowsEvents.end();
	}
} // namespace nabi
