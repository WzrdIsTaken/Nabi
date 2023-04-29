#pragma once

#include <string>
#include <vector>

#include "TypeUtils.h"

namespace nabi::Utils::StringUtils
{
#define TOSTRING(thing) #thing
#define CONCAT(A, B) A ## B

	// Can't have a function for this which allocates on the stack cos otherwise the returned string ptr will be invalid 
#define STRING_TO_LPCWSTR(string, lpcwstr) \
	std::wstring const wtemp = std::wstring(string.begin(), string.end()); \
    lpcwstr = wtemp.c_str();

	// A LPCSTR is just a typedef'ed WCHAR
#define STRING_TO_WCHAR(string, wchar) STRING_TO_LPCWSTR(string, wchar)

	// Splits a string via a delimiter, after a character
	[[nodiscard]] std::vector<std::string_view> SplitString(std::string_view const string, char const after, char const delimiter, size_t const expectedValues = SIZE_MAX) NABI_NOEXCEPT;
	// Splits a string via a delimiter. Fatal assert if expectedValues is not equal to the resultant split
	[[nodiscard]] std::vector<std::string_view> SplitString(std::string_view const string, char const delimiter, size_t const expectedValues = SIZE_MAX) NABI_NOEXCEPT;

	// Trims a string
    static inline char const* const c_WhiteSpace = " \t\n\r\f\v";

	inline std::string& RightTrim(std::string& string, char const* trim = c_WhiteSpace) NABI_NOEXCEPT
	{
		string.erase(string.find_last_not_of(trim) + 1);
		return string;
	}
	inline std::string& LeftTrim(std::string& string, char const* trim = c_WhiteSpace) NABI_NOEXCEPT
	{
		string.erase(0, string.find_first_not_of(trim));
		return string;
	}
	inline std::string& Trim(std::string& string, char const* trim = c_WhiteSpace) NABI_NOEXCEPT
	{
		return LeftTrim(RightTrim(string, trim), trim);
	}
} // namespace nabi::Utils::StringUtils
