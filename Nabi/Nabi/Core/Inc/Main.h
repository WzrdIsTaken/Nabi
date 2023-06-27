#pragma once
#include "Core.h"

#include "shellapi.h" // For CommandLineToArgvW

#include "Console.h"
#include "InitSettings.h"
#include "NabiCore.h"

namespace nabi
{
	struct NabiMainParams final
	{
        NabiCoreSettings m_CoreSettings;
	};

	struct WinMainParams final
	{
        HINSTANCE m_hInstance;
        HINSTANCE m_hPrevInstance;
        LPWSTR m_lpCmdLine;
        int m_nShowCmd;
	};

    template<typename T>
    [[nodiscard]] int Main(NabiMainParams const& nabiParams, WinMainParams const& winParams) NABI_NOEXCEPT
    {
		STATIC_ASSERT(std::is_base_of<NabiCore, T>::value, "Main's templated parameter must inherit from NabiCore");

		// --- Application Setup ---

		// Get argc + argv
		int argc;
		LPWSTR* argv = CommandLineToArgvW(winParams.m_lpCmdLine, &argc); // Prev used GetCommandLineW()

		// --- Initial Nabi Setup ---

		DX_ASSERT(CoInitializeEx(nullptr, COINIT_MULTITHREADED));

		// Create a console window
#ifdef USE_DEBUG_UTILS
		UINT constexpr consoleMaxLines = 4096u;
		nabi::AllocateConsole(consoleMaxLines);
#endif // #ifdef USE_DEBUG_UTILS

		// Setup Debug Utils
#ifdef USE_DEBUG_UTILS
	// Without calling this function, the assert dialogue window doesn't appear
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);

		// Set up the logger
		using nabi::DebugUtils::Logger;
		Logger::CreateInstance();
#endif // #ifdef USE_DEBUG_UTILS

		// Seed random
		nabi::MathUtils::SeedRandom();

		// --- Run Tests ---

		// Run Tests
#ifdef RUN_TESTS
	// Set the log level to warning because otherwise it spams the console when tests are run
#ifdef USE_DEBUG_UTILS
		Logger::Instance()->SetLogLevel(LOG_WARN);
#endif // #ifdef USE_DEBUG_UTILS

		// Run all tests
		::testing::InitGoogleTest(&argc, argv);
		int const testResults = RUN_ALL_TESTS();

		// Assert if any of the tests failed
		ASSERT(testResults == NABI_SUCCESS, "One or more of the tests failed! See the console output for details, or run the test explorer.");

		// Set the log level back to all
#ifdef USE_DEBUG_UTILS
		Logger::Instance()->SetLogLevel(LOG_INFO);
#endif // #ifdef USE_DEBUG_UTILS

		LOG(NEWLINE << LOG_PREP, LOG_INFO, LOG_CATEGORY_CORE, "All tests run. Initializing Nabi..." << NEWLINE, LOG_END);
#endif // #ifdef RUN_TESTS

		// --- Init Nabi ---

		auto app = T(winParams.m_hInstance, nabiParams.m_CoreSettings);
		LOG(NEWLINE << LOG_PREP, LOG_INFO, LOG_CATEGORY_CORE, "Nabi has initialized successfully!" << NEWLINE, LOG_END);

		int appRunResult = app.Init();
		ASSERT(appRunResult == NABI_SUCCESS, "The app failed to initialize!");

		appRunResult = app.Run();
		ASSERT(appRunResult == NABI_SUCCESS, "The app hit an error while running!");

		// --- Shutdown Nabi ---

		// Destroy the console
#ifdef USE_DEBUG_UTILS
		nabi::ReleaseConsole();
#endif // #ifdef USE_DEBUG_UTILS

		CoUninitialize();

		LOG(NEWLINE << LOG_PREP, LOG_INFO, LOG_CATEGORY_CORE, "Shutting down Nabi with code " << appRunResult, LOG_END);
		return appRunResult;
    }
} // namespace nabi
