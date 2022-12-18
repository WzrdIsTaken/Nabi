#include "../../TestCore.h"

#include "../../../Utils/UtilClasses/Logger.h"

#ifdef _DEBUG

namespace nabitest::DebugTests
{
	// Test that the logger is initialized
	TEST(DebugTests, CheckLoggerIsInitialized)
	{
		// Grab the logger, and check that an instance has been created
		using nabi::Utils::DebugUtils::Logger;
		bool const instanceValid = Logger::IsInstanceValid();

		ASSERT_TRUE(instanceValid);
	}

	// Test that logging works corretly
	TEST(DebugTests, CheckLogCalls)
	{
		// Grab the logger and cache the expected number of messages logged
		using nabi::Utils::DebugUtils::Logger;
		Logger::LogCount const expectedMessagesLogged = Logger::Instance()->GetLogCount() + 1ull;

		// I know we could use EXPECT_CALL here, but this is easier...
		Comparison<Logger::LogCount> comparison(expectedMessagesLogged);
		LOG(LOG_PREP, LOG_INFO, "Hello Nabi!");

		// Check that everything lines up
		comparison.m_Actual = Logger::Instance()->GetLogCount();
		COMPAIR_EQ(comparison);
	}

	// Idea - Could write tests for asserts?
}

#endif // #ifdef _DEBUG