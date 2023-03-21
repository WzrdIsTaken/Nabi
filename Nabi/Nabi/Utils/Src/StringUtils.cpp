#include "StringUtils.h"

#include "DebugUtils.h"

namespace nabi::Utils::StringUtils
{
	std::vector<std::string_view> SplitString(std::string_view const string, char const after, char const delimiter, size_t const expectedValues) NABI_NOEXCEPT
	{
		size_t const afterPos = string.find(after) + 1u;
		std::string_view const afterString = string.substr(afterPos);

		return SplitString(afterString, delimiter, expectedValues);
	}

	std::vector<std::string_view> SplitString(std::string_view const string, char const delimiter, size_t const expectedValues) NABI_NOEXCEPT
	{
		std::vector<std::string_view> result;
		result.reserve(std::count(string.begin(), string.end(), delimiter) + 1);

		for (std::string_view::const_iterator end = string.begin();; ++end)
		{
			std::string_view::const_iterator const start = end;
			end = std::find(end, string.end(), delimiter);

			result.emplace_back(start, end);
			if (end == string.end())
			{
				ASSERT(expectedValues != SIZE_MAX && result.size() == expectedValues, "Tried to split the string, but there wasn't the expected number of substrings!");
				return result;
			}
		}
	}
} // namespace nabi::Utils::StringUtils