#include "Core.h"

int main()
{
	// --- Initial Nabi Setup and Tests ---

	// Setup Debug Utils
#ifdef USE_DEBUG_UTILS
	// Without calling this function, the assert dialogue window doesn't appear
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);

	// Set up the logger
	using nabi::Utils::DebugUtils::Logger;
	Logger::CreateInstance();

	// Set the log level to error because otherwise it spams the console when tests are run
	Logger::Instance()->SetLogLevel(LOG_ERROR);
#endif // #ifdef USE_DEBUG_UTILS

	// Run Tests
#ifdef RUN_TESTS
	// Run all tests
	::testing::InitGoogleTest(&__argc, __argv);
	int const testResults = RUN_ALL_TESTS();

	// Assert if any of the tests failed
	using namespace nabi::Utils::TestUtils;
	ASSERT(testResults == c_TestResultSuccess, "One or more of the tests failed! See the console output for details, or run the test explorer.");
#endif // #ifdef RUN_TESTS

	// Reset the logger's state
#ifdef USE_DEBUG_UTILS
	// Set the log level back to all
	Logger::Instance()->SetLogLevel(LOG_LEVEL_ALL);
#endif // USE_DEBUG_UTILS

	// --- Init Nabi ---

	LOG(NEWLINE << LOG_PREP, LOG_INFO, "Nabi has initialized successfully!" << ENDLINE);
	return 0;
}
