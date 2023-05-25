#include "TestCore.h"

#include "CoreComponents\ColliderComponent.h"
#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreSingletonComponents\CollisionStateComponent.h"
#include "CoreSystems\CollisionSystem.h"

#ifdef RUN_TESTS

namespace nabitest::PhysicsTests
{
	struct TestCollisionEntitySettings final
	{
		dx::XMFLOAT3 m_Position;
	};

	entt::entity CreateTestCollisionEntity(nabi::Context& context, TestCollisionEntitySettings const& settings)
	{
		entt::entity entity = context.m_Registry.create();

		ecs::TransformComponent transformComponent = {};
		transformComponent.m_Position = settings.m_Position;
		transformComponent.m_Rotation = { 0.0f, 0.0f, 0.0f };
		transformComponent.m_Scale    = { 0.0f, 0.0f, 0.0f };

		ecs::RigidbodyComponent rigidbodyComponent = {};

		ecs::ColliderComponent colliderComponent = {};
		colliderComponent.m_ColliderType = ecs::ColliderComponent::ColliderType::Cube;
		colliderComponent.m_InteractionType = ecs::ColliderComponent::InteractionType::Dynamic;
		colliderComponent.m_ColliderDimensions = { 0.625f, 0.625f, 0.625f };
		colliderComponent.m_Mask = 1 << 1;

		context.m_Registry.emplace<ecs::TransformComponent>(entity, transformComponent);
		context.m_Registry.emplace<ecs::RigidbodyComponent>(entity, rigidbodyComponent);
		context.m_Registry.emplace<ecs::ColliderComponent> (entity, colliderComponent );

		return entity;
	}

	void TickAndExpect(ecs::CollisionSystem& collisionSystem, ecs::SComp::CollisionStateComponent const& collisionStateComponent, size_t const expected)
	{
		nabi::GameTime gameTime = {};
		collisionSystem.Update(gameTime);

		Comparison<size_t> collidingEntities(expected);
		collidingEntities.m_Actual = collisionStateComponent.m_CurrentCollisions.size();

		COMPAIR_EQ(collidingEntities);
	}

	// Check that the CollisionSystem detects two intersecting AABBs
	TEST(PhysicsTests, CheckCollisions)
	{
		// Mock objects
		nabi::Context context;
		ecs::CollisionSystem collisionSystem(context, "id"_hs, "group"_hs);

		entt::entity const corePhysicsEntity = context.m_Registry.create();
		auto const& collisionStateComponent = context.m_Registry.emplace<ecs::SComp::CollisionStateComponent>(corePhysicsEntity);
		context.m_SingletonEntites.at(nabi::Context::SingletonEntities::Physics) = corePhysicsEntity;

		// Create two entities with rb/col components
		TestCollisionEntitySettings testEntityOneSettings;
		testEntityOneSettings.m_Position = { -1.0f, 0.0f, 0.0f };
		entt::entity const testEntityOne = CreateTestCollisionEntity(context, testEntityOneSettings);

		TestCollisionEntitySettings testEntityTwoSettings;
		testEntityTwoSettings.m_Position = { 1.0f, 0.0f, 0.0f };
		entt::entity const testEntityTwo = CreateTestCollisionEntity(context, testEntityTwoSettings);

		// Tick the physics system, expect no collisions
		TickAndExpect(collisionSystem, collisionStateComponent, 0u);

		// Move the entities on top of each other
		context.m_Registry.get<ecs::TransformComponent>(testEntityOne).m_Position = { 0.0f, 0.0f, 0.0f };
		context.m_Registry.get<ecs::TransformComponent>(testEntityTwo).m_Position = { 0.0f, 0.0f, 0.0f };

		// Tick the physics system, expect a collision
		TickAndExpect(collisionSystem, collisionStateComponent, 1u);

		// Move the entities apart again
		context.m_Registry.get<ecs::TransformComponent>(testEntityOne).m_Position = {  1.0f, 0.0f, 0.0f };
		context.m_Registry.get<ecs::TransformComponent>(testEntityTwo).m_Position = { -1.0f, 0.0f, 0.0f };

		// Tick the physics system, expect no collisons
		TickAndExpect(collisionSystem, collisionStateComponent, 0u);

		// Move the entities together and change the collision mask of one of them
		context.m_Registry.get<ecs::TransformComponent>(testEntityOne).m_Position = { 0.0f, 0.0f, 0.0f };
		context.m_Registry.get<ecs::TransformComponent>(testEntityTwo).m_Position = { 0.0f, 0.0f, 0.0f };
		context.m_Registry.get<ecs::ColliderComponent>(testEntityOne).m_Mask = 1 << 2;

		// Tick the physics system, expect no collisons
		TickAndExpect(collisionSystem, collisionStateComponent, 0u);

		// Finally, change the entities mask to all. 
		context.m_Registry.get<ecs::ColliderComponent>(testEntityOne).m_Mask = ~0;

		// Expect a collision
		TickAndExpect(collisionSystem, collisionStateComponent, 1u);
	}
} // namespace nabitest::PhysicsTests

#endif // #ifdef RUN_TESTS
