#pragma once

#include <string>

#include "entt.h"

#include "TypeUtils.h"

namespace nabi::ECSUtils
{
	[[nodiscard]] std::string GetEntityUUIDAsString(entt::entity const entity) NABI_NOEXCEPT;
} // namespace nabi::ECSUtils
