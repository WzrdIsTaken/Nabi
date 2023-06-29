#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Systems\AsteroidSystem.h"

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
		// TODAY
		// Need to write an asset bank for the Demo. Need the functions which will turn eg ModelComponent to buffer components

		// so on the bad side... a lot of todos..
		// but on the good side - im glad we are doing this now. come on ben - you got this!!
	}
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
