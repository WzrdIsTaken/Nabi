#pragma once

namespace nabitests
{
	template<typename T>
	struct Comparison
	{
		Comparison()
		{
		}

		Comparison(T expected)
			: m_Expected(expected)
			, m_Actual()
		{
		}

		Comparison(T expected, T actual)
			: m_Expected(expected)
			, m_Actual(actual)
		{
		}

		T m_Expected;
		T m_Actual;
	};
} // namespace nabitest
