#pragma once
#include "EngineCore.h"
#include "WinCore.h"

#include "InitSettings.h"

namespace nabi
{
	class Window final
	{
	public:
		Window(HINSTANCE const hInstance, WindowSettings const& settings) NABI_NOEXCEPT;
		~Window();

		void AddMessageSubscriber(UINT const messageId, std::function<void(WPARAM, LPARAM)> const subscriber) NABI_NOEXCEPT;
		void RemoveMessageSubscriber(UINT const messageId, std::function<void(WPARAM, LPARAM)> const subscriber) NABI_NOEXCEPT;

		/// <summary>
		/// TODO - Note how this func is static because it should process messages for all windows
		/// TODO - const up stuff here
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

		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;
		static LRESULT CALLBACK HandleMsgProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) NABI_NOEXCEPT;

		std::vector<std::pair<UINT, std::function<void(WPARAM, LPARAM)>>> m_MessageSubscribers;
		HWND m_hWnd;

		HINSTANCE const c_hInstance;
		LPCWSTR const c_WindowClassName;
	};
} // namespace nabi::Rendering
