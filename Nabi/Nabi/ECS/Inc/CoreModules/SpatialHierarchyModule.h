#pragma once
#include "Core.h"

namespace ecs::SpatialHierarchyModule
{
	void ForeachEntityChild(entt::registry& registry, entt::entity const entity, std::function<bool(entt::entity const)> const& action);
} // namespace ecs::SpatialHierarchyModule
