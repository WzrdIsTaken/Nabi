#pragma once
#include "Core.h"

namespace ecs
{
	struct ColliderComponent final : public nabi::ECS::ComponentBase
	{
		enum class ColliderType : int
		{
			Sphere,
			// others

			ENUM_COUNT
		};

		ColliderType m_ColliderType;
	};
} // namespace ecs
