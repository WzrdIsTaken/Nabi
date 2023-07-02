#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Systems\AsteroidSystem.h"

#include "CoreComponents\TransformComponent.h"
#include "CoreModules\EntityModule.h"
#include "CoreModules\InputModule.h"
#include "DirectXUtils.h"

#include "Demo\Core\AssetTypes.h"
#include "Demo\Core\Demo.h"
#include "Demo\ECS\Components\AsteroidComponent.h"
#include "Demo\ECS\SingletonComponents\DemoPropertiesComponent.h"

namespace ecs
{
	REFLECT_SYSTEM_BEGIN_DEFAULT(AsteroidSystem)
	RELFECT_SYSTEM_END(AsteroidSystem)

	AsteroidSystem::AsteroidSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(AsteroidSystem)
	}

	AsteroidSystem::~AsteroidSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(AsteroidSystem)
	}

	void AsteroidSystem::Update(nabi::GameTime const& gameTime)
	{
		float const dt = static_cast<float>(gameTime.GetDeltaTime());

		RotateAsteroids(dt);
		CheckInput();
	}

	void AsteroidSystem::RotateAsteroids(float const dt) const
	{
		m_Context.m_Registry.view<TransformComponent, AsteroidComponent const>().each(
			[&](auto& transformComponent, auto& asteroidComponent) -> void
			{
				dx::XMFLOAT3 rotationSpeed = nabi::DirectXUtils::Float3Multiply(asteroidComponent.m_SpinDirection, asteroidComponent.m_SpinSpeed);
				rotationSpeed = nabi::DirectXUtils::Float3Multiply(rotationSpeed, dt);

				transformComponent.m_Rotation = nabi::DirectXUtils::Float3Add(transformComponent.m_Rotation, rotationSpeed);
			});
	}

	void AsteroidSystem::CheckInput() const
	{
		auto const& demoPropertiesComponent = EntityModule::GetSingletonComponent<SComp::DemoPropertiesComponent>(m_Context);
		auto const loadAsteroidGroupKeyState = InputModule::GetKeyboardKey(m_Context, demoPropertiesComponent.m_LoadAsteroidGroupKey);
		auto const unloadAsteroidGroupKeyState = InputModule::GetKeyboardKey(m_Context, demoPropertiesComponent.m_UnloadAsteroidGroupKey);

		if (loadAsteroidGroupKeyState == nabi::Input::InputState::Pressed)
		{
			m_Context.m_EntityCreator->CreateEntityGroup(c_AsteroidGroupName.data());
			reinterpret_cast<core::Demo* const>(m_Context.m_CorePointer)->RefreshLoadedAssets(core::AssetType::Model); // See Demo.cpp::RefreshLoadedAssets for an explanation

		}
		if (unloadAsteroidGroupKeyState == nabi::Input::InputState::Pressed)
		{
			m_Context.m_EntityCreator->DestroyEntityGroup(c_AsteroidGroupName.data());
		}
	}
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
