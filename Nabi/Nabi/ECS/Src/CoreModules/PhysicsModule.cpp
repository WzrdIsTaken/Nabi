#include "Core.h"

#include "CoreModules\PhysicsModule.h"

#include "AABB.h"
#include "CollisionSolvers.h"

namespace ecs::PhysicsModule
{
	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis)
	{
		nabi::Physics::AABB lhsAABB = {};
		nabi::Physics::AABB rhsAABB = {};

		return GetSortedCollisionView(context, comparisonAxis, lhsAABB, rhsAABB);
	}

	CollisionView GetSortedCollisionView(nabi::Context& context, SComp::CollisionStateComponent::MaxVariance const comparisonAxis,
		nabi::Physics::AABB& lhsAABB, nabi::Physics::AABB& rhsAABB)
	{
		auto view = context.m_Registry.view<TransformComponent, RigidbodyComponent, ColliderComponent const>();
		view.storage<TransformComponent const>().sort(
			[&context, comparisonAxis, &lhsAABB, &rhsAABB](entt::entity const lhs, entt::entity const rhs)
			{
				TransformComponent const& lhsTransform = context.m_Registry.get<TransformComponent>(lhs);
				TransformComponent const& rhsTransform = context.m_Registry.get<TransformComponent>(rhs);

				ColliderComponent const& lhsCollider = context.m_Registry.get<ColliderComponent>(lhs);
				ColliderComponent const& rhsCollider = context.m_Registry.get<ColliderComponent>(rhs);

				ReassignAABBFromCollisionComponents(lhsAABB, lhsTransform, lhsCollider);
				ReassignAABBFromCollisionComponents(rhsAABB, rhsTransform, rhsCollider);

				float const lhsMaxVariance = GetVarianceValue(lhsAABB.m_MinExtents, comparisonAxis);
				float const rhsMaxVariance = GetVarianceValue(rhsAABB.m_MinExtents, comparisonAxis);

				return lhsMaxVariance < rhsMaxVariance;
			});
		view = view.use<TransformComponent>();

		return view;
	}

	void ReassignAABBFromCollisionComponents(nabi::Physics::AABB& aabb,
		TransformComponent const& transformComponent, ColliderComponent const& colliderComponent)
	{
		dx::XMFLOAT3 const& center = transformComponent.m_Position;
		dx::XMFLOAT3 const& dimensions = colliderComponent.m_ColliderDimensions;

		using namespace nabi::Physics;
		CollisionSolvers::ReassignAABBFromCenter(aabb, center, dimensions);

		switch (colliderComponent.m_ColliderType)
		{
		case ColliderComponent::ColliderType::Cube:
			// No special case 
			break;
		case ColliderComponent::ColliderType::Sphere:
		{
			ASSERT(dimensions.x == dimensions.y && dimensions.y == dimensions.z,
				"For a sphere collider, expecting x/y/z dimensions to be equal");

			float const radius = dimensions.x / 2.0f;
			CollisionSolvers::MakeAABBIntoSphere(aabb, radius);
			break;
		}
		default:
			ASSERT_FAIL("Using an unexpected ColliderComponent::ColliderType");
			break;
		}
	}

	float GetVarianceValue(dx::XMFLOAT3 const& extent, SComp::CollisionStateComponent::MaxVariance const variance)
	{
		// high quality function

		using namespace nabi::TypeUtils;
		int const maxVarianceIndex = ToUnderlying<SComp::CollisionStateComponent::MaxVariance>(variance);

		ASSERT_FATAL(maxVarianceIndex == 0 || maxVarianceIndex == 1 || maxVarianceIndex == 2,
			"Indexing into a float3 with a value of " << maxVarianceIndex << " is *not* going to go well.");

		char* const ptr = reinterpret_cast<char*>(&const_cast<dx::XMFLOAT3&>(extent));
		float const maxVariance = *reinterpret_cast<float*>(ptr + sizeof(float) * maxVarianceIndex);

		return maxVariance;
	}
} // namespace ecs::PhysicsModule
