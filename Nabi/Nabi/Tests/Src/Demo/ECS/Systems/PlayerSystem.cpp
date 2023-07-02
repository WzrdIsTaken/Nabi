#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Systems\PlayerSystem.h"

#include "CoreComponents\RigidbodyComponent.h"
#include "CoreComponents\TransformComponent.h"
#include "CoreModules\InputModule.h"
#include "CoreModules\CameraModule.h"

#include "Demo\ECS\Components\PlayerComponent.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(PlayerSystem)
	RELFECT_SYSTEM_END(PlayerSystem)

	PlayerSystem::PlayerSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PlayerSystem)
	}

	PlayerSystem::~PlayerSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(PlayerSystem)
	}

	void PlayerSystem::Update(nabi::GameTime const& gameTime)
	{
		float const dt = static_cast<float>(gameTime.GetDeltaTime());

		m_Context.m_Registry.view<PlayerComponent const, ecs::RigidbodyComponent, ecs::TransformComponent const>()
			.each([&](auto& playerComponent, auto& rigidbodyComponent, auto& transformComponent)
				{
					MovePlayer(dt, playerComponent, rigidbodyComponent);

					auto& camera = CameraModule::GetMainPerspectiveCameraComponent(m_Context); 
					camera.m_Position = transformComponent.m_Position;
				});
	}

	void PlayerSystem::MovePlayer(float const dt, PlayerComponent const& playerComponent, RigidbodyComponent& rigidbodyComponent) const
	{
		using namespace nabi::Input;

		InputState const fwdKeyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_ForwardKey);
		InputState const bwdKeyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_BackwardKey);
		InputState const lefteyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_LeftKey);
		InputState const rightKeyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_RightKey);
		InputState const upKeyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_UpKey);
		InputState const downKeyState = InputModule::GetKeyboardKey(m_Context, playerComponent.m_DownKey);

		if (fwdKeyState == InputState::Pressed)
		{
			rigidbodyComponent.m_Velocity.y += playerComponent.m_Speed * dt;
		}
		if (bwdKeyState == InputState::Pressed)
		{
			rigidbodyComponent.m_Velocity.y -= playerComponent.m_Speed * dt;
		}
		
		// TODO - if there a better way of writing this logic?
	}
} // namespace ecs

#endif INCLUDE_DEMO
