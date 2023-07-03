#pragma once
#include "Core.h"

namespace ecs
{
	struct CameraComponent;
} // namespace ecs

namespace ecs
{
	class CameraSystem final : nabi::ECS::SystemBase
	{
	public:
		CameraSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId);
		~CameraSystem();

		void Update(nabi::GameTime const& gameTime);

	private:
		void CheckAndUpdateCamera(CameraComponent& cameraComponent) const;
		void UpdateCamera(CameraComponent& cameraComponent) const;

		REFLECT_PRIVATES(CameraSystem)
	};
} // namespace ecs
