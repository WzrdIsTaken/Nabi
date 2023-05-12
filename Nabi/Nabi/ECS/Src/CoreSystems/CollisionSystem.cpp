#include "Core.h"

#include "CoreSystems\CollisionSystem.h"

#include "CollisionSolvers.h"
#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "DebugUtils.h"
#include "DirectXUtils.h"
#include "TypeUtils.h"

namespace ecs
{
	CollisionSystem::CollisionSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(CollisionSystem)
	}

	CollisionSystem::~CollisionSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(CollisionSystem)
	}

	void CollisionSystem::Update(nabi::GameTime const& gameTime)
	{
		float const dt = static_cast<float>(gameTime.GetDeltaTime()); // FixedDeltaTime...

		BroadPhase(/*dt? to pass to NarrowPhase()*/);
	}

	// 

	void CollisionSystem::BroadPhase() const
	{
		// Sweep and prune collision detection

		using namespace nabi::Physics;
		using namespace nabi::Utils;

		// Sweep along the comparison axis. This axis (x, y or z) is the axis on which in the previous frame there was the largest
		// distance between colliders. Using this axis to sweep can help ensure that the spatial partitioning is balanced 
		// and reduce unnecessary intersection tests.
		SComp::CollisionStateComponent::MaxVariance const comparisonAxis = GetCollisionStateComponent().m_MaxVarianceAxis;

		// Left and right hand side AABBs, used throughout the loop
		AABB lhsAABB = {};
		AABB rhsAABB = {};

		// Sort the view by the AABB's transforms along the comparison axis
		auto view = m_Context.m_Registry.view<TransformComponent, RigidbodyComponent const, ColliderComponent const>();
		view.storage<TransformComponent const>().sort(
			[&](entt::entity const lhs, entt::entity const rhs)
			{
				TransformComponent const& lhsTransform = m_Context.m_Registry.get<TransformComponent>(lhs);
				TransformComponent const& rhsTransform = m_Context.m_Registry.get<TransformComponent>(rhs);

				ColliderComponent const& lhsCollider = m_Context.m_Registry.get<ColliderComponent>(lhs);
				ColliderComponent const& rhsCollider = m_Context.m_Registry.get<ColliderComponent>(rhs);

				ReassignAABBFromCollisionComponents(lhsAABB, lhsTransform, lhsCollider);
				ReassignAABBFromCollisionComponents(rhsAABB, rhsTransform, rhsCollider);

				float const lhsMaxVariance = GetVarianceValue(lhsAABB.m_MinExtents, comparisonAxis);
				float const rhsMaxVariance = GetVarianceValue(rhsAABB.m_MinExtents, comparisonAxis);

				return lhsMaxVariance < rhsMaxVariance;
			});
		view = view.use<TransformComponent>();
		auto each = view.each();

		// The center is used to calculate the next comparison axis
		dx::XMFLOAT3 centerSum = DirectXUtils::c_Float3Zero;
		dx::XMFLOAT3 centerSumSquared = DirectXUtils::c_Float3Zero;

		// Iterate through each collider
		size_t iterationProgress = 1u;
		for (auto lhsIt = each.begin(); lhsIt != each.end(); ++lhsIt)
		{
			// Get the AABB lhs collider
			auto [lhsEntity, lhsTransformComponent, lhsRigidbodyComponent, lhsColliderComponent] = *lhsIt;
			ReassignAABBFromCollisionComponents(lhsAABB, lhsTransformComponent, lhsColliderComponent);

			dx::XMFLOAT3 const center = AABBSolvers::GetCenter(lhsAABB);
			centerSum = DirectXUtils::Float3Add(centerSum, center);
			centerSumSquared = DirectXUtils::Float3Add(centerSumSquared, DirectXUtils::Float3Square(center));

			// Iterate through each collider in front of the lhs one
			auto rhsIt = each.begin();
			rhsIt = std::next(rhsIt, iterationProgress); // iterationProgress - 1u? or because we are no longer using a conventional for loop this is ok?

			for (; rhsIt != each.end(); ++rhsIt)
			{
				// Get the AABB rhs collider
				auto [rhsEntity, rhsTansformComponent, rhsRigidbodyComponent, rhsColliderComponent] = *rhsIt;
				ReassignAABBFromCollisionComponents(rhsAABB, rhsTansformComponent, rhsColliderComponent);

				// Check if the AABBs are intersecting
				dx::XMFLOAT3 const& otherMinExtents = lhsAABB.m_MinExtents;
				dx::XMFLOAT3 const& thisMaxExtents = rhsAABB.m_MaxExtents;
				float const otherMinExtent = GetVarianceValue(otherMinExtents, comparisonAxis);
				float const thisMaxExtent = GetVarianceValue(thisMaxExtents, comparisonAxis);

				bool const otherExtentBeyondThisExtent = otherMinExtent > thisMaxExtent;
				if (otherExtentBeyondThisExtent)
				{
					// No more collisions are possible with this AABB
					break;
				}

				bool const aabbsIntersect = AABBSolvers::Intersects(lhsAABB, rhsAABB);
				if (aabbsIntersect)
				{
					NarrowPhase();
				}
			}

			++iterationProgress;
		}

		CalculateNextMaxVariance(iterationProgress, centerSum, centerSumSquared);
	}

	void CollisionSystem::NarrowPhase() const
	{
		int i = 0;
		i++;
	}

	//

	float CollisionSystem::GetVarianceValue(dx::XMFLOAT3 const& extent, SComp::CollisionStateComponent::MaxVariance const variance) const
	{
		// high quality function

		using namespace nabi::Utils::TypeUtils;
		int const maxVarianceIndex = ToUnderlying<SComp::CollisionStateComponent::MaxVariance>(variance);

		ASSERT_FATAL(maxVarianceIndex == 0 || maxVarianceIndex == 1 || maxVarianceIndex == 2, 
			"Indexing into a float3 with a value of " << maxVarianceIndex << " is *not* going to go well.");

		char* const ptr = reinterpret_cast<char*>(&const_cast<dx::XMFLOAT3&>(extent));
		float const maxVariance = *reinterpret_cast<float*>(ptr + sizeof(float) * maxVarianceIndex);

		return maxVariance;
	}

	void CollisionSystem::CalculateNextMaxVariance(size_t const numberOfColliders, dx::XMFLOAT3 const& centerSum, dx::XMFLOAT3 const& centerSumSquared) const
	{
		using namespace nabi::Utils;

		float const numberOfCollidersAsFloat = static_cast<float>(numberOfColliders);
		dx::XMFLOAT3 const centerSumDivSize = DirectXUtils::Float3Divide(centerSum, numberOfCollidersAsFloat);
		dx::XMFLOAT3 const centerSumSquaredDivSize = DirectXUtils::Float3Divide(centerSumSquared, numberOfCollidersAsFloat);

		dx::XMFLOAT3 const variance = DirectXUtils::Float3Subtract(centerSumSquaredDivSize, DirectXUtils::Float3Square(centerSumDivSize));
		float maxVariance = variance.x;
		auto maxVarianceAxis = SComp::CollisionStateComponent::MaxVariance::X;

		if (variance.y > maxVariance)
		{
			maxVariance = variance.y;
			maxVarianceAxis = SComp::CollisionStateComponent::MaxVariance::Y;
		}
		if (variance.z > maxVariance)
		{
			maxVariance = variance.z;
			maxVarianceAxis = SComp::CollisionStateComponent::MaxVariance::Z;
		}

		GetCollisionStateComponent().m_MaxVarianceAxis = maxVarianceAxis;
	}

	void CollisionSystem::ReassignAABBFromCollisionComponents(nabi::Physics::AABB& aabb, TransformComponent const& transformComponent, ColliderComponent const& colliderComponent) const
	{
		dx::XMFLOAT3 const& center = transformComponent.m_Position;
		dx::XMFLOAT3 const& dimensions = colliderComponent.m_ColliderDimensions;

		nabi::Physics::AABBSolvers::ReassignAABBFromCenter(aabb, center, dimensions);
	}

	// 

	SComp::CollisionStateComponent& CollisionSystem::GetCollisionStateComponent() const
	{
		entt::entity physicsEntity = m_Context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Physics);
		return m_Context.m_Registry.get<SComp::CollisionStateComponent>(physicsEntity);
	}
} // namespace ecs
