#include "TestCore.h"

#ifdef RUN_TESTS

namespace nabitest::ReflectionTests
{
	// Enum -> String functionality is currently not needed, I will implement if it becomes needed!

	// Check enums work as expected
	TEST(ReflectionTests, ParseEnum)
	{
		//Comparison<std::string> nameComparison("Ben");
		Comparison<MockEnum> valueComparison(MockEnum::Kyra);

		//nameComparison.m_Actual = nabi::Reflection::EnumConverter::EnumToString(20);
		valueComparison.m_Actual = nabi::Reflection::EnumConverter::StringToEnum<MockEnum>("Kyra"); // Wooyoung <3

		//COMPAIR_EQ(nameComparison);
		COMPAIR_EQ(valueComparison);
	}

	// Check enum flags work as expected
	TEST(ReflectionTests, ParseEnumFlags)
	{
		MockEnumFlags const mockFlags = nabi::Reflection::EnumConverter::StringToEnum<MockEnumFlags>("One|Two");
		MockEnumFlags const mockAll   = nabi::Reflection::EnumConverter::StringToEnum<MockEnumFlags>("All"    );

		EXPECT_TRUE (static_cast<bool> (mockFlags & MockEnumFlags::One  ));
		EXPECT_TRUE (static_cast<bool> (mockAll   & MockEnumFlags::Two  ));
		EXPECT_FALSE(static_cast<bool> (mockFlags & MockEnumFlags::Three));
	}
} // namespace nabitest::ReflectionTests

#endif // #ifdef RUN_TESTS
