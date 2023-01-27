#pragma once

#include <string>

#include "../../Libraries/Inc/entt/entt.h"
#include "TypeUtils.h"

namespace nabi::Utils::ECSUtils
{
	[[nodiscard]] std::string GetEntityUUIDAsString(entt::entity const entity) NABI_NOEXCEPT;
} // namespace nabi::Utils::ECSUtils
