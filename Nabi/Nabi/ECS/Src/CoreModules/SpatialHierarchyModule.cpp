#include "Core.h"

#include "CoreModules\SpatialHierarchyModule.h"

#include "CoreComponents\SpatialHierarchyComponent.h"

namespace ecs::SpatialHierarchyModule
{
	void ForeachEntityChild(entt::registry& registry, entt::entity const entity, std::function<bool(entt::entity const)> const& action)
	{
		SpatialHierarchyComponent& spatialHierarchyComponent = registry.get<SpatialHierarchyComponent>(entity);
		ForeachEntityChild(registry, spatialHierarchyComponent.m_Children, action);
	}

	void ForeachEntityChild(entt::registry& registry, std::vector<entt::entity> const& children, std::function<bool(entt::entity const)> const& action)
	{
		for (entt::entity child : children)
		{
			bool const continueLooping = action(child);
			if (!continueLooping)
			{
				break;
			}
		}
	}
} // namespace ecs::SpatialHierarchyModule
