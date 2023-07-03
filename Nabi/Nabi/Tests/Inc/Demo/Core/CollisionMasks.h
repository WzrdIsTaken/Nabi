#pragma once
#include "Demo\DemoCore.h"

#include "CoreComponents\ColliderComponent.h"

namespace core
{
	// Note: Matching masks will collide

	enum class CollisionMask : ecs::ColliderComponent::ColliderMask
	{
		Object = 1 << 1,
		Player = 1 << 1, 
		All = ~0,
		ENUM_COUNT
	};
	DEFINE_ENUM_FLAG_OPERATORS(CollisionMask)
} // namespace core
