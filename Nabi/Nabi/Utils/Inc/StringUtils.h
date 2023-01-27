#pragma once

#include <string>
#include <vector>

#include "TypeUtils.h"

namespace nabi::Utils::StringUtils
{
	[[nodiscard]] std::vector<std::string_view> SplitString(std::string_view const string, char const delimiter, size_t const expectedValues = SIZE_MAX) NABI_NOEXCEPT;
} // nabi::Utils::StringUtils
