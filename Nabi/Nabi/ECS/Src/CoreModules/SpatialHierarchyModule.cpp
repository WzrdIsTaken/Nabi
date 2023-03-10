#include "Core.h"

#include "CoreModules\SpatialHierarchyModule.h"

#include "CoreComponents\SpatialHierarchyComponent.h"

namespace ecs::SpatialHierarchyModule
{
	void ForeachEntityChild(entt::registry& registry, entt::entity const entity, std::function<bool(entt::entity const)> const& action)
	{
		SpatialHierarchyComponent& spatialHierarchyComponent = registry.get<SpatialHierarchyComponent>(entity);
		for (entt::entity child : spatialHierarchyComponent.m_Children)
		{
			bool const continueLooping = action(child);
			if (!continueLooping)
			{
				break;
			}
		}
	}
} // namespace ecs::SpatialHierarchyModule
