#pragma once
#include "Core.h"

namespace ecs
{
	struct ColliderComponent final : public nabi::ECS::ComponentBase
	{
		enum class ColliderType : int
		{
			Sphere,
			Cube,

			ENUM_COUNT
		};

		ColliderType m_ColliderType;
		dx::XMFLOAT3 m_ColliderDimensions;
	};
} // namespace ecs
