#include "Core.h"

#include "CoreSystems\PhysicsSystem.h"

#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "DirectXUtils.h"

namespace ecs
{
	using namespace nabi::Utils::DirectXUtils;

	PhysicsSystem::PhysicsSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PhysicsSystem)
	}

	PhysicsSystem::~PhysicsSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PhysicsSystem)
	}

	void PhysicsSystem::Update(nabi::GameTime const& gameTime)
	{
		float const dt = static_cast<float>(gameTime.GetDeltaTime());

		m_Context.m_Registry.view<TransformComponent, RigidbodyComponent>()
			.each([&](auto& transformComponent, auto& rigidbodyComponent)
				{
					SetPosition(transformComponent, rigidbodyComponent);
					SetRotation(transformComponent, rigidbodyComponent);

					ComputeVelocity(rigidbodyComponent, dt);
					ComputeAngularVelocity(rigidbodyComponent, dt);
				});
	}

	void PhysicsSystem::SetPosition(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent) const
	{
		dx::XMFLOAT3 const newPosition = Float3Add(transformComponent.m_Position, rigidbodyComponent.m_Velocity);
		SetTransformValues(transformComponent.m_Position, newPosition, rigidbodyComponent.m_PositionConstraints);
	}

	void PhysicsSystem::SetRotation(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent) const
	{
		dx::XMFLOAT3 const newRotation = Float3Add(transformComponent.m_Rotation, rigidbodyComponent.m_AngularVelocity);
		SetTransformValues(transformComponent.m_Rotation, newRotation, rigidbodyComponent.m_RotationConstraints);
	}

	void PhysicsSystem::ComputeVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const
	{
		// todo write this | physics system should use dx, user shouldnt need to times things by dt for physics operations
	}

	void PhysicsSystem::ComputeAngularVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const
	{
		// todo write this | physics system should use dx, user shouldnt need to times things by dt for physics operations
	}

	void PhysicsSystem::SetTransformValues(dx::XMFLOAT3& values, dx::XMFLOAT3 const& newValues, RigidbodyComponent::Constraints const& valueConstraints) const
	{
		if (!valueConstraints.m_FreezeX) values.x = newValues.x;
		if (!valueConstraints.m_FreezeY) values.y = newValues.y;
		if (!valueConstraints.m_FreezeZ) values.z = newValues.z;
	}
}
