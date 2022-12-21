#include "Core.h"

int main()
{
	// Setup assert dialogue
#ifdef _DEBUG
	// Without calling this function, the assert dialogue window doesn't appear
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_WNDW);
#endif // #ifdef _DEBUG

	// Run Tests
#ifdef _DEBUG
	// Loooong namespaces
	using nabi::Utils::DebugUtils::Logger;
	using namespace nabi::Utils::TestUtils;

	// Create the logger
	Logger::CreateInstance();
	Logger::Instance()->SetLogLevel(LOG_ERROR);

	// Run all tests
	::testing::InitGoogleTest(&__argc, __argv);
	int const testResults = RUN_ALL_TESTS();

	// Assert if any of the tests failed
	ASSERT(testResults == c_TestResultSuccess, "One or more of the tests failed! See the console output for details, or run the test explorer.");

	// Set the log level back to all (it was disabled because otherwise it spams the console during the tests)
	Logger::Instance()->SetLogLevel(LOG_LEVEL_ALL);
#endif // #ifdef _DEBUG

	LOG(NEWLINE << LOG_PREP, LOG_INFO, "Nabi has initialized successfully!" << ENDLINE);
	return 0;
}
