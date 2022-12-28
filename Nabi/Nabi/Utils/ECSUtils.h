#pragma once

#include <string>

#include "../Libraries/entt/entt.h"

namespace nabi::Utils::ECSUtils
{
	inline std::string GetEntityUUIDAsString(entt::entity const entity)
	{
		// Entt is already tracking a UUID for each entity, so we might as well use it
		// The id for each entity is guaranteed to be unique, and will be recycled if the entity is destroyed

#ifdef USE_DEBUG_UTILS
		// Just to avoid any unexpected surprises...
		static_assert(std::is_same<ENTT_ID_TYPE, uint32_t>::value, "Was expecting ENTT_ID_TYPE to be a uint32!");
#endif // #ifdef USE_DEBUG_UTILS

		ENTT_ID_TYPE const entityId = static_cast<ENTT_ID_TYPE>(entity);
		return std::to_string(entityId);
	}
} // namespace nabi::Utils::ECSUtils
