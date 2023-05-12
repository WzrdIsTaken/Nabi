#pragma once
#include "Core.h"

#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"

namespace ecs::SComp
{
	struct CollisionStateComponent final : public nabi::ECS::ComponentBase
	{
		enum class MaxVariance : int
		{
			X = 0, 
			Y = 1, 
			Z = 2,

			ENUM_COUNT
		};

		MaxVariance m_MaxVarianceAxis;
	};
} // namespace ecs::SComp
