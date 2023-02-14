/*
	                 ,;;,.                  ,;'',
                    /~\                    /~\
                   ([~])                  ([~])
                 ,_.~~~.                  .~~~.
               ()--|   ,\                /    ,\    ()
            ,_//   |   |>)              (<|   |\()--'m
         (~'  m''~)(   )/                \(   )   ~~'|
          \(~||~)/ //~\\                  //~\\     ||
             ||   ()   ()                ()   () /( || )\
             ||   ||   ||                ||   ||( '-||-' )
             || ,;.)   (.;,            ,;.)   (.;,(~\/~)/

	These are the header guards. They guard every header in this project, keeping the lands of Nabi safe.
*/

#include "Core.h"

#include "shellapi.h" // For CommandLineToArgvW

#include "Console.h"
#include "InitSettings.h"
#include "NabiCore.h"

int CALLBACK wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	// --- Application Setup ---

	// Get argc/argv
	int argc;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	// --- Initial Nabi Setup ---

	using namespace nabi;
	using namespace nabi::Utils;

	// Create a console window
#ifdef USE_DEBUG_UTILS
	UINT const consoleMaxLines = 4096u;
	AllocateConsole(consoleMaxLines);
#endif // #ifdef USE_DEBUG_UTILS

	// Setup Debug Utils
#ifdef USE_DEBUG_UTILS
	// Without calling this function, the assert dialogue window doesn't appear
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);

	// Set up the logger
	using DebugUtils::Logger;
	Logger::CreateInstance();
#endif // #ifdef USE_DEBUG_UTILS

	// Seed random
	MathUtils::SeedRandom();

	// --- Run Tests ---

	// Run Tests
#ifdef RUN_TESTS
	// Set the log level to error because otherwise it spams the console when tests are run
#ifdef USE_DEBUG_UTILS
	Logger::Instance()->SetLogLevel(LOG_ERROR);
#endif // #ifdef USE_DEBUG_UTILS

	// Run all tests
	::testing::InitGoogleTest(&argc, argv);
	int const testResults = RUN_ALL_TESTS();

	// Assert if any of the tests failed
	ASSERT(testResults == NABI_SUCCESS, "One or more of the tests failed! See the console output for details, or run the test explorer.");

	// Set the log level back to all
#ifdef USE_DEBUG_UTILS
	Logger::Instance()->SetLogLevel(LOG_LEVEL_ALL);
#endif // #ifdef USE_DEBUG_UTILS
#endif // #ifdef RUN_TESTS

	// --- Init Nabi ---

	nabi::NabiCore app = nabi::NabiCore(hInstance, nabi::nabiCoreDefaultSettings);
	LOG(NEWLINE << LOG_PREP, LOG_INFO, "Nabi has initialized successfully!" << ENDLINE);

	int appRunResult = app.Init();
	ASSERT(appRunResult == NABI_SUCCESS, "The app failed to initialize!");

	appRunResult = app.Run();
	ASSERT(appRunResult == NABI_SUCCESS, "The app hit an error while running!");

	// --- Shutdown Nabi ---

	// Destroy the console
#ifdef USE_DEBUG_UTILS
	ReleaseConsole();
#endif // #ifdef USE_DEBUG_UTILS

	LOG(NEWLINE << LOG_PREP, LOG_INFO, "Shutting down Nabi with an appRunResult of " << WRAP(appRunResult, "'") << ENDLINE);
	return appRunResult;
}
