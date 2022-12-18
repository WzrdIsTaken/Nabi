#include "Core.h"

int main()
{
	// Run Tests
#ifdef _DEBUG
	// Loooong namespaces
	using nabi::Utils::DebugUtils::Logger;
	using namespace nabi::Utils::TestUtils;

	// Create the logger
	Logger::CreateInstance();
	Logger::Instance()->SetLogLevel(Logger::c_LogLevelNone);

	// Run all tests
	::testing::InitGoogleTest(&__argc, __argv);
	int const testResults = RUN_ALL_TESTS();

	// Assert if any of the tests failed
	ASSERT(testResults == c_TestResultSuccess, "One or more of the tests failed! See the console output for details, or run the test explorer.");

	// Set the log level back to all (we disabled it because otherwise it spams the console during the tests)
	Logger::Instance()->SetLogLevel(Logger::c_LogLevelAll);
#endif // #ifdef _DEBUG

	LOG(LOG_PREP, LOG_INFO, "Nabi has initialized successfully!" << ENDLINE);
	return 0;
}
