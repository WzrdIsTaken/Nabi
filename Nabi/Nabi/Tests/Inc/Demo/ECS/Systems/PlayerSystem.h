#pragma once
#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

namespace ecs
{
	struct PlayerComponent;
	struct RigidbodyComponent;
	struct TransformComponent;
} // namespace ecs

namespace ecs
{
	class PlayerSystem final : nabi::ECS::SystemBase
	{
	public:
		PlayerSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~PlayerSystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		void MovePlayer(float const dt, PlayerComponent const& playerComponent, RigidbodyComponent& rigidbodyComponent) const;
		void RotatePlayer(float const dt, PlayerComponent const& playerComponent, RigidbodyComponent& rigidbodyComponent) const;
		void RaycastFromPlayer(PlayerComponent const& playerComponent, TransformComponent const& transformComponent) const;

		REFLECT_PRIVATES(PlayerSystem)
	};
} // namespace ecs

#endif INCLUDE_DEMO
