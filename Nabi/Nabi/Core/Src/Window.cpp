#include "EngineCore.h"

#include "Window.h"

#include "DebugUtils.h"

namespace nabi
{
	Window::Window(HINSTANCE const hInstance, WindowSettings const& settings) NABI_NOEXCEPT
		: m_hWnd(nullptr)
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

		m_hWnd = CreateWindow(
			c_WindowClassName,                        // Window class
			settings.m_WindowName,                    // Window text
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // Window style

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
	}

	Window::~Window()
	{
		DX_ASSERT(DestroyWindow(m_hWnd));
		DX_ASSERT(UnregisterClass(c_WindowClassName, c_hInstance));
	}

	void Window::AddMessageSubscriber(UINT const messageId, std::function<void(WPARAM, LPARAM)> const subscriber) NABI_NOEXCEPT
	{
		// TODO - ASSERT IF MESSAGE - SUBSCRIBER PAIR ALREADY EXISTS
		m_MessageSubscribers.emplace_back(messageId, subscriber);
	}

	void Window::RemoveMessageSubscriber(UINT const messageId, std::function<void(WPARAM, LPARAM)> const subscriber) NABI_NOEXCEPT
	{
		// TODO - ASSERT IF MESSAGE - SUBSCRIBER PAIR DOESN'T EXIST
		// TOOD - ACC WRITE THIS FUNCTION

		// TODO - I think the event system can be replaced with Entts. So do this when intergrating
		// https://github.com/skypjack/entt/wiki/Crash-Course:-events,-signals-and-everything-in-between#signals
		// Then might acc be able to write this function

		//auto const subscriberPair = std::make_pair(messageId, subscriber);
		//auto const it = std::find(m_MessageSubscribers.begin(), m_MessageSubscribers.end(), subscriberPair);
		//m_MessageSubscribers.erase(it);
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
		// Iterate through the subscribers, and notify them of any messages
		for (auto [message, subscriber] : m_MessageSubscribers)
		{
			if (message == msg)
			{
				subscriber(wParam, lParam);
			}
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
} // namespace nabi
