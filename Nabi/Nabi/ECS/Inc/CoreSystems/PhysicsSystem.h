#pragma once
#include "Core.h"

#include "PhysicsConstants.h"

namespace ecs
{
	struct RigidbodyComponent;
	struct TransformComponent;
} // namespace ecs

namespace ecs
{
	class PhysicsSystem final : public nabi::ECS::SystemBase
	{
	public:
		PhysicsSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~PhysicsSystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		void SetPosition(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent, float const dt) const;
		void SetRotation(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent, float const dt) const;

		void ComputeVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const;
		void ComputeAngularVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const;

		static dx::XMFLOAT3 constexpr c_Gravity = nabi::Physics::Constants::c_Gravity;

		REFLECT_PRIVATES(PhysicsSystem)
	};
} // namespace ecs