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
#endif // #ifdef USE_DEBUG_UTILS

	// Run Tests
#ifdef RUN_TESTS
	// Set the log level to error because otherwise it spams the console when tests are run
#ifdef USE_DEBUG_UTILS
	Logger::Instance()->SetLogLevel(LOG_ERROR);
#endif // USE_DEBUG_UTILS

	// Run all tests
	::testing::InitGoogleTest(&__argc, __argv);
	int const testResults = RUN_ALL_TESTS();

	// Assert if any of the tests failed
	using namespace nabi::Utils::TestUtils;
	ASSERT(testResults == c_TestResultSuccess, "One or more of the tests failed! See the console output for details, or run the test explorer.");

	// Set the log level back to all
#ifdef USE_DEBUG_UTILS
	Logger::Instance()->SetLogLevel(LOG_LEVEL_ALL);
#endif // USE_DEBUG_UTILS
#endif // #ifdef RUN_TESTS

	// --- Init Nabi ---

	LOG(NEWLINE << LOG_PREP, LOG_INFO, "Nabi has initialized successfully!" << ENDLINE);
	return 0;
}
