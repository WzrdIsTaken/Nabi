#include "Core.h"

int main()
{
#ifdef _DEBUG
	// TODO - Disable logging

	::testing::InitGoogleTest(&__argc, __argv);
	const int testResults = RUN_ALL_TESTS();

	ASSERT(testResults == nabi::Utils::TestUtils::c_TestResultSuccess, "One or more of the tests failed! See the console output for details, or run the test explorer.");

	// TODO - Enable logging
#endif // #ifdef _DEBUG

	// TODO - Seperate tests define?
	// TODO - Fix google test warnings (or suppress)
	// TODO - Find and fix that incomplete type not allowed

    std::cout << "Hello Nabi!\n";
	return 0;
}
