#pragma once
#include "Core.h"

namespace ecs
{
	struct SpatialHierarchyComponent final : public nabi::ECS::ComponentBase
	{
		entt::entity m_Parent;
		std::vector<entt::entity> m_Children;
	};
} // namespace ecs
