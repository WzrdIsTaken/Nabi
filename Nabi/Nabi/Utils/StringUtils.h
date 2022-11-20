#pragma once

#include <string>
#include <vector>

#include "DebugUtils.h"

namespace nabi::Utils::StringUtils
{
	inline std::vector<std::string_view> SplitString(std::string_view const string, char const delimiter, size_t const expectedValues = SIZE_MAX)
	{
		std::vector<std::string_view> result;
		result.reserve(std::count(string.begin(), string.end(), delimiter) + 1);

		for (std::string_view::const_iterator end = string.begin();; ++end)
		{
			std::string_view::const_iterator start = end;
			end = std::find(end, string.end(), delimiter);

			result.emplace_back(start, end);
			if (end == string.end())
			{
				ASSERT_FATAL(expectedValues != SIZE_MAX && result.size() == expectedValues, "Tried to split the string, but there wasn't the expected number of substrings!");
				return result;
			}
		}
	}
} // nabi::Utils::StringUtils
