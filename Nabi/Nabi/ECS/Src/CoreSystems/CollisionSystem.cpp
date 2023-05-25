#include "Core.h"

#include "CoreSystems\CollisionSystem.h"

#include "Collision.h"
#include "CollisionSolvers.h"
#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\ReflectionModule.h"
#include "DirectXUtils.h"
#include "ECSUtils.h"

// For some games, narrow phase collision resolution might not be needed - just collision detection
#define ENABLE_NARROW_PHASE 

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
		float const dt = static_cast<float>(gameTime.GetFixedDeltaTime());
		BroadPhase(dt);
	}

	// 

	void CollisionSystem::BroadPhase(float const dt) const
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
		auto view = m_Context.m_Registry.view<TransformComponent, RigidbodyComponent, ColliderComponent const>();
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

			dx::XMFLOAT3 const center = CollisionSolvers::GetCenter(lhsAABB);
			centerSum = DirectXUtils::Float3Add(centerSum, center);
			centerSumSquared = DirectXUtils::Float3Add(centerSumSquared, DirectXUtils::Float3Square(center));

			// Iterate through each collider in front of the lhs one
			auto rhsIt = each.begin();
			rhsIt = std::next(rhsIt, iterationProgress);

			for (; rhsIt != each.end(); ++rhsIt)
			{
				// Check if the two colliders have a valid mask to check collisions
				auto [rhsEntity, rhsTransformComponent, rhsRigidbodyComponent, rhsColliderComponent] = *rhsIt;
				bool const validMask = static_cast<bool>(lhsColliderComponent.m_Mask & rhsColliderComponent.m_Mask);

				if (validMask)
				{
					// Get the AABB rhs collider
					ReassignAABBFromCollisionComponents(rhsAABB, rhsTransformComponent, rhsColliderComponent);
					LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION <<
						CollisionSolvers::AABBToString(lhsAABB, "LHS:") + " | " + CollisionSolvers::AABBToString(rhsAABB, "RHS:") << ENDLINE);

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

					bool const aabbsIntersect = CollisionSolvers::Intersects(lhsAABB, rhsAABB);
					CollisionState collisionState = CollisionState::NotColliding;

					if (aabbsIntersect)
					{
						ASSERT_CODE
						(
							static uint64_t collisionCount = 0ull;
							++collisionCount;
							LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION << "Broad Phase - Collision! Count: " << collisionCount << ENDLINE);
						)

#ifdef ENABLE_NARROW_PHASE
						// Proceed to narrow phase collision
						NarrowPhaseData lhsNarrowPhaseData = {
							lhsAABB, lhsColliderComponent, lhsRigidbodyComponent, lhsTransformComponent
						};
						NarrowPhaseData rhsNarrowPhaseData = {
							rhsAABB, rhsColliderComponent, rhsRigidbodyComponent, rhsTransformComponent
						};
						NarrowPhase(dt, lhsNarrowPhaseData, rhsNarrowPhaseData);
#endif // ifdef ENABLE_NARROW_PHASE

						// Set the collision state to colliding, used below for firing collision events
						collisionState = CollisionState::Colliding;
					}

					// Fire collision events
					CollisionEventData const lhsCollisionEventData = {
						lhsColliderComponent, lhsEntity
					};
					CollisionEventData const rhsCollisionEventData = {
						rhsColliderComponent, rhsEntity
					};
					FireCollisionEvents(collisionState, lhsCollisionEventData, rhsCollisionEventData);
				}
			}

			++iterationProgress;
		}

		CalculateNextMaxVariance(iterationProgress, centerSum, centerSumSquared);
	}

	void CollisionSystem::NarrowPhase(float const dt, NarrowPhaseData& lhsData, NarrowPhaseData& rhsData) const
	{
		using namespace nabi::Physics;

		auto narrowPhaseHelper =
			[](AABB const& lhsAABB, AABB const& rhsAABB) -> Collision
			{
				Collision collision = {};
				collision.m_Normal = CollisionSolvers::CalculateCollisionNormal(lhsAABB, rhsAABB);
				
				dx::XMFLOAT3 const depth = CollisionSolvers::CalculatePenetrationDepth(lhsAABB, rhsAABB);
				collision.m_Depth = CollisionSolvers::CalculateSmallestPentrationDepth(depth);

				return collision;
			};

		if (lhsData.m_Collider.m_InteractionType == ColliderComponent::InteractionType::Dynamic)
		{
			Collision collision = narrowPhaseHelper(rhsData.m_AABB, lhsData.m_AABB);
			ResolveCollision(dt, collision, lhsData);
		}
		if (rhsData.m_Collider.m_InteractionType == ColliderComponent::InteractionType::Dynamic)
		{
			Collision collision = narrowPhaseHelper(lhsData.m_AABB, rhsData.m_AABB);
			ResolveCollision(dt, collision, rhsData);
		}
	}

	void CollisionSystem::FireCollisionEvents(CollisionState const collisionState, CollisionEventData const& lhsData, CollisionEventData const& rhsData) const
	{
		ASSERT_CODE(using namespace nabi::Utils::ECSUtils;)

		auto fireCollisionEventsHelper =
			[&](entt::hashed_string const& actionType, entt::hashed_string const& actionName, entt::entity const lhsEntity, entt::entity const rhsEntity) -> void
			{
				ReflectionModule::Constraints constexpr constraints = ReflectionModule::c_EventConstraints;
				ReflectionModule::CallReflectedFunction(m_Context, actionType, actionName, &constraints, entt::forward_as_meta(m_Context), lhsEntity, rhsEntity);
			};

		SComp::CollisionStateComponent::CurrentCollisions& currentCollisions = GetCollisionStateComponent().m_CurrentCollisions;
		SComp::CollisionStateComponent::CollisionPair const collisionPair =
		{
			lhsData.m_Entity,
			rhsData.m_Entity,
		};

		ColliderComponent const& lhsCollider = lhsData.m_Collider;
		ColliderComponent const& rhsCollider = rhsData.m_Collider;
		
		auto it = std::find(currentCollisions.begin(), currentCollisions.end(), collisionPair);
		bool const presentInCurrentCollisions = it != currentCollisions.end();

		if (collisionState == CollisionState::Colliding         && !presentInCurrentCollisions)
		{
			LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION << "Collision Enter: " <<
				GetEntityUUIDAsString(lhsData.m_Entity) << "-" << GetEntityUUIDAsString(rhsData.m_Entity) << ENDLINE);

			currentCollisions.emplace_back(collisionPair);

			fireCollisionEventsHelper(lhsCollider.m_OnCollisionEnterType, lhsCollider.m_OnCollisionEnterAction, lhsData.m_Entity, rhsData.m_Entity);
			fireCollisionEventsHelper(rhsCollider.m_OnCollisionEnterType, rhsCollider.m_OnCollisionEnterAction, rhsData.m_Entity, lhsData.m_Entity);
		}
		else if (collisionState == CollisionState::NotColliding && presentInCurrentCollisions)
		{
			LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION << "Collision Exit: " <<
				GetEntityUUIDAsString(lhsData.m_Entity) << "-" << GetEntityUUIDAsString(rhsData.m_Entity) << ENDLINE);

			currentCollisions.erase(it); // this isn't using a std::remove itr, but is fine here i think? https://stackoverflow.com/a/24011727

			fireCollisionEventsHelper(lhsCollider.m_OnCollisionExitType, lhsCollider.m_OnCollisionExitAction, lhsData.m_Entity, rhsData.m_Entity);
			fireCollisionEventsHelper(rhsCollider.m_OnCollisionExitType, rhsCollider.m_OnCollisionExitAction, rhsData.m_Entity, lhsData.m_Entity);
		}
	}

	void CollisionSystem::ResolveCollision(float const dt, nabi::Physics::Collision const& collision, NarrowPhaseData& data) const
	{
		using namespace nabi::Utils;

		dx::XMFLOAT3 const resultant = DirectXUtils::Float3Multiply(collision.m_Normal, collision.m_Depth);
		float const ms = 1.0f / dt;
		LOG(LOG_PREP, LOG_TRACE, "Narrow Phase - Normal: " << DirectXUtils::Float3ToString(collision.m_Normal) <<
			" | Depth: " << collision.m_Depth << " | Normal * Depth = " << DirectXUtils::Float3ToString(resultant) << ENDLINE);

		// Transform adjustment
		TransformComponent& transform = data.m_TransformComponent;
		transform.m_Position = DirectXUtils::Float3Add(transform.m_Position, resultant);

		// Rigidbody adjustment 
		RigidbodyComponent& rigidbody = data.m_RigidbodyComponent;
		dx::XMFLOAT3 const velocityChange = DirectXUtils::Float3Multiply(resultant, ms);
		rigidbody.m_Velocity = DirectXUtils::Float3Add(rigidbody.m_Velocity, velocityChange);
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

	void CollisionSystem::ReassignAABBFromCollisionComponents(nabi::Physics::AABB& aabb, 
		TransformComponent const& transformComponent, ColliderComponent const& colliderComponent) const
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

	// 

	SComp::CollisionStateComponent& CollisionSystem::GetCollisionStateComponent() const
	{
		entt::entity const physicsEntity = m_Context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Physics);
		return m_Context.m_Registry.get<SComp::CollisionStateComponent>(physicsEntity);
	}
} // namespace ecs

/*
* Could be useful in the future? 
* I used this in NarrowPhase() before the refactor
ASSERT_CODE
(

	using namespace nabi::Utils::DirectXUtils;
	LOG(LOG_PREP, LOG_TRACE, LOG_CATEGORY_COLLISION << "Narrow Phase - LHS Collision Normal: " << Float3ToString(lhsCollision.m_Normal)
		<< " | Depth: " << Float3ToString(lhsCollisionDepth) <<    " - RHS Collision Normal: " << Float3ToString(rhsCollision.m_Normal)
		<< " | Depth: " << Float3ToString(rhsCollisionDepth) << ENDLINE);
)
*/
