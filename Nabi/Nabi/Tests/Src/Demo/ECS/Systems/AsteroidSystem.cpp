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
		// - Need a ToString function for types, eg for XMFLOAT3, which turns them into a relfection friendly string repretentation
		//	- should this be a reflected func?
		//  - or just a templated one with that new approach idea?
		// - I think the reason that OnDemoBegin isn't being called is because the sytems only get constructed at the end of the reflection?
		//	- gotta think of a nice solution for this
		// - And have to sort out how we are going to store game singletoncomponents

		// so on the bad side... a lot of todos..
		// but on the good side - im glad we are doing this now. come on ben - you got this!!

		// Need to write an asset bank for the Demo. Need the functions which will turn eg ModelComponent to buffer components
	}
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
