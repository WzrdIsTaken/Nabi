#include "Core.h"

#include "CoreSystems\PhysicsSystem.h"

#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "DirectXUtils.h"

namespace ecs
{
	PhysicsSystem::PhysicsSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PhysicsSystem)
	}

	PhysicsSystem::~PhysicsSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PhysicsSystem)
	}

	void PhysicsSystem::Update(nabi::GameTime const& /*gameTime*/)
	{
		float dt = 0.001f;
		// I think its finally time to write a GameTime
		// do setpos/rot need dt?

		// i have no idea if this setvelocity is any good. probs not
		// can the set/compute funcs just be made into one and set params?

		// yeah gotta work out these set funcs. will write game time and intergrate it, then come back to this and think harder

		m_Context.m_Registry.view<TransformComponent, RigidbodyComponent>()
			.each([&](auto& transformComponent, auto& rigidbodyComponent)
				{
					SetPosition(transformComponent, rigidbodyComponent, dt);
					SetRotation(transformComponent, rigidbodyComponent, dt);

					ComputeVelocity(rigidbodyComponent, dt);
					ComputeAngularVelocity(rigidbodyComponent, dt);
				});
	}

	void PhysicsSystem::SetPosition(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent, float const /*dt*/) const
	{
		using namespace nabi::Utils::DirectXUtils;

		dx::XMFLOAT3 const newPosition = Float3Add(transformComponent.m_Position, rigidbodyComponent.m_Velocity);
		transformComponent.m_Position = newPosition;
	}

	void PhysicsSystem::SetRotation(TransformComponent& transformComponent, RigidbodyComponent const& rigidbodyComponent, float const /*dt*/) const
	{
		using namespace nabi::Utils::DirectXUtils;


	}

	void PhysicsSystem::ComputeVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const
	{
		using namespace nabi::Utils::DirectXUtils;

		dx::XMFLOAT3 const gravity = Float3Multiply(c_Gravity, rigidbodyComponent.m_GravityScale);
		dx::XMFLOAT3 const movementViaGravity = Float3Multiply(gravity, rigidbodyComponent.m_Mass);

		dx::XMFLOAT3 const resistance = Float3Add(rigidbodyComponent.m_Drag, rigidbodyComponent.m_Mass);
		dx::XMFLOAT3 const movementViaVelocity = Float3Add(rigidbodyComponent.m_Velocity, resistance); // <-- this is defo wrong. i think??

		dx::XMFLOAT3 const totalChange = Float3Add(movementViaVelocity, movementViaGravity);
		dx::XMFLOAT3 const velocityChange = Float3Multiply(totalChange, dt);

		dx::XMFLOAT3 const newVelocity = Float3Add(rigidbodyComponent.m_Velocity, velocityChange);
		rigidbodyComponent.m_Velocity = newVelocity;
	}

	void PhysicsSystem::ComputeAngularVelocity(RigidbodyComponent& rigidbodyComponent, float const dt) const
	{
		using namespace nabi::Utils::DirectXUtils;


	}
}
