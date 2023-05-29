#pragma once
#include "Core.h"

#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreSingletonComponents\CollisionStateComponent.h"
#include "RaycastHitResult.h"

namespace nabi::Physics
{
	struct AABB;
} // namespace nabi::Physics

namespace ecs::PhysicsModule
{
	typedef entt::view<entt::get_t<TransformComponent, RigidbodyComponent, ColliderComponent const>> CollisionView;

	struct RaycastSettings final
	{
		float m_Range;
		ColliderComponent::ColliderMask m_Mask;
	};
	RaycastSettings constexpr c_DefaultRaycastSettings
	{
		.m_Range = FLT_MAX,
		.m_Mask = ~0ull
	};

	inline SComp::CollisionStateComponent const& GetCollisionStateComponent(nabi::Context const& context)
	{
		entt::entity const physicsEntity = context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Physics);
		return context.m_Registry.get<SComp::CollisionStateComponent>(physicsEntity);
	}
	inline SComp::CollisionStateComponent& GetCollisionStateComponent(nabi::Context& context)
	{
		return const_cast<SComp::CollisionStateComponent&>(GetCollisionStateComponent(const_cast<nabi::Context const&>(context)));
	}

	inline bool ValidCollisionMask(nabi::Context const& /*context*/, ColliderComponent::ColliderMask const lhsMask, ColliderComponent::ColliderMask const rhsMask)
	{
		return static_cast<bool>(lhsMask & rhsMask);
	}

	nabi::Physics::RaycastHitResult Raycast(nabi::Context& context, dx::XMFLOAT3 const& origin, dx::XMFLOAT3 const& direction, RaycastSettings const& settings);
	std::vector<nabi::Physics::RaycastHitResult> Raycast(nabi::Context& context, dx::XMFLOAT3 const& origin, dx::XMFLOAT3 const& direction, int const numberOfResults, RaycastSettings const& settings);

	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis);
	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis, nabi::Physics::AABB& lhsAABB, nabi::Physics::AABB& rhsAABB);

	void ReassignAABBFromCollisionComponents(nabi::Physics::AABB& aabb, TransformComponent const& transformComponent, ColliderComponent const& colliderComponent);
	float GetVarianceValue(dx::XMFLOAT3 const& extent, SComp::CollisionStateComponent::MaxVariance const variance);
} // namespace ecs::PhysicsModule