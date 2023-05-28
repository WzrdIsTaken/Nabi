#pragma once
#include "Core.h"

#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreSingletonComponents\CollisionStateComponent.h"

namespace nabi::Physics
{
	struct AABB;
} // namespace nabi::Physics

namespace ecs::PhysicsModule
{
	typedef entt::view<entt::get_t<TransformComponent, RigidbodyComponent, ColliderComponent const>> CollisionView;

	inline SComp::CollisionStateComponent const& GetCollisionStateComponent(nabi::Context const& context)
	{
		entt::entity const physicsEntity = context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Physics);
		return context.m_Registry.get<SComp::CollisionStateComponent>(physicsEntity);
	}
	inline SComp::CollisionStateComponent& GetCollisionStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::CollisionStateComponent&>(GetCollisionStateComponent(const_cast<nabi::Context const&>(context)));
	}

	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis);
	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis, nabi::Physics::AABB& lhsAABB, nabi::Physics::AABB& rhsAABB);

	void ReassignAABBFromCollisionComponents(nabi::Physics::AABB& aabb, TransformComponent const& transformComponent, ColliderComponent const& colliderComponent);
	float GetVarianceValue(dx::XMFLOAT3 const& extent, SComp::CollisionStateComponent::MaxVariance const variance);
} // namespace ecs::PhysicsModule
