#pragma once

#ifdef RUN_TESTS

namespace nabitest
{
	// Why is this a marco? Tbh I just like the different colour compaired to the rest of the code and think it makes the tests asserts stand out
	// The main problem I wanted to solve was accidentally typing m_Expected/m_Actual twice, and these marcos do that still
	// Is it kinda just rewriting gtest macros? kinda.. but idk i had that typo and it cost me like 10 mins! never again xD 
	// I need to be able to trust the tests! not be double checking them...

#define COMPAIR(comparison, comparisonType) comparison.Compair(comparisonType);
#define COMPAIR_EQ(comparison) COMPAIR(comparison, ComparisonType::Equal);

	// The comparisons supported (well, that google test supports and that I have made a roundabout way of using xd)
	enum class ComparisonType
	{
		Equal,
		ENUM_COUNT
	};

	/// <summary>
	/// Basic comparison struct
	/// </summary>
	/// <typeparam name="T">The type to compair</typeparam>
	template<typename T>
	struct Comparison final
	{
		Comparison()
			: m_Expected{}
			, m_Actual{}
		{
		}

		explicit Comparison(T expected)
			: m_Expected(expected)
			, m_Actual{}
		{
		}

		Comparison(T expected, T actual)
			: m_Expected(expected)
			, m_Actual(actual)
		{
		}

		/// <summary>
		/// Runs the gtest EXPECT func which corresponds to comparisonType against m_Expected and m_Actual
		/// </summary>
		/// <param name="comparisonType">The type of EXPECT_[comparison] which will be called</param>
		void Compair(ComparisonType const comparisonType)
		{
			switch (comparisonType)
			{
			case ComparisonType::Equal:
				EXPECT_EQ(m_Expected, m_Actual);
				break;
			default:
				ASSERT_FAIL("ComparisonType comparison is not implemented!");
				break;
			}
		}

		T m_Expected;
		T m_Actual;
	};
} // namespace nabitest

#endif // #ifdef RUN_TESTS
