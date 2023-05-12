#pragma once
#include "Core.h"

#include "CoreSingletonComponents\CollisionStateComponent.h"

namespace ecs
{
	struct TransformComponent;
} // namespace ecs
namespace nabi::Physics
{
	struct AABB;
} // namespace nabi::Physics

namespace ecs
{
	class CollisionSystem final : public nabi::ECS::SystemBase
	{
	public:
		CollisionSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~CollisionSystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		void BroadPhase() const;
		void NarrowPhase() const;

		float GetVarianceValue(dx::XMFLOAT3 const& extent, SComp::CollisionStateComponent::MaxVariance const variance) const;
		void CalculateNextMaxVariance(size_t const numberOfColliders, dx::XMFLOAT3 const& centerSum, dx::XMFLOAT3 const& centerSumSquared) const;

		void ReassignAABBFromCollisionComponents(nabi::Physics::AABB& aabb, TransformComponent const& transformComponent, ColliderComponent const& colliderComponent) const;

		SComp::CollisionStateComponent& GetCollisionStateComponent() const;

		REFLECT_PRIVATES(CollisionSystem)
	};
} // namespace ecs
