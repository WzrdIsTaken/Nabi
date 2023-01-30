#pragma once

#include <string>

#include "entt.h"

#include "TypeUtils.h"

namespace nabi::Utils::ECSUtils
{
	[[nodiscard]] std::string GetEntityUUIDAsString(entt::entity const entity) NABI_NOEXCEPT;
} // namespace nabi::Utils::ECSUtils
