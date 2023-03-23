#pragma once
#include "WinCore.h"

namespace nabi
{
	// Window Settings
	struct WindowSettings final
	{
		LONG m_Width;
		LONG m_Height;
		LPCWSTR m_WindowName;
		LPCWSTR m_ClassName;
	};

	WindowSettings const windowDefaultSettings = {
		.m_Width = 1280l,
		.m_Height = 720l,
		.m_WindowName = L"Nabi",
		.m_ClassName = L"Butterfly"
	};

	// Nabi Core Settings
	struct NabiCoreSettings final
	{
		WindowSettings m_WindowSettings;
	};

	NabiCoreSettings const nabiCoreDefaultSettings = {
		.m_WindowSettings = windowDefaultSettings
	};
} // namespace nabi
