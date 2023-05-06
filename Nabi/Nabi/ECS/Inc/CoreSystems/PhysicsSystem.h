#pragma once
#include "Core.h"

#include "CoreComponents\RigidbodyComponent.h" // Can't forward declare nested classes ):
#include "PhysicsConstants.h"

namespace ecs
{
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
		void SetPosition(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent) const;
		void SetRotation(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent) const;

		void ComputeVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const;
		void ComputeAngularVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const;

		void SetTransformValues(dx::XMFLOAT3& values, dx::XMFLOAT3 const& newValues, RigidbodyComponent::Constraints const& valueConstraints) const;

		static dx::XMFLOAT3 constexpr c_Gravity = nabi::Physics::Constants::c_Gravity;

		REFLECT_PRIVATES(PhysicsSystem)
	};
} // namespace ecs