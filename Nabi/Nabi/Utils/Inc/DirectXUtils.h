#pragma once
#include "DirectXCore.h" // TODO - Forward declare matrix?

#include <string>

#include "TypeUtils.h"

namespace nabi::Utils::DirectXUtils
{
	[[nodiscard]] std::string MatrixToString(dx::XMMATRIX const& matrix) NABI_NOEXCEPT;
} // namespace nabi::Utils::DirectXUtils
