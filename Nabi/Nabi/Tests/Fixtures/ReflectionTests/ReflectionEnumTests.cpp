#include "TestCore.h"

#ifdef RUN_TESTS

namespace nabitest::ReflectionTests
{
	// Check enums work as expected
	TEST(ReflectionTests, ParseEnum)
	{
		// Enum -> String functionality is currently not needed, I will implement if it becomes needed!

		//Comparison<std::string> nameComparison("Ben");
		Comparison<MockEnum> valueComparison(MockEnum::Kyra);

		//nameComparison.m_Actual = nabi::Reflection::EnumConverter::EnumToString(20);
		valueComparison.m_Actual = nabi::Reflection::EnumConverter::StringToEnum<MockEnum>("Kyra"); // Wooyoung <3

		//COMPAIR_EQ(nameComparison);
		COMPAIR_EQ(valueComparison);
	}
} // namespace nabitest::ReflectionTests

#endif // #ifdef RUN_TESTS
