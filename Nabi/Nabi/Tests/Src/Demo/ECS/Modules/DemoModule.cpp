#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Modules\DemoModule.h"

namespace ecs::DemoModule
{
	// TODAY
	// 
	// IF NOT GOING TO FINISH THE DEMO BEFORE THE 7TH - JUST FOCUS ON TODOS. I WANT A CLEAR WEEK BEFORE KR TO RELAX AND PREP
	// 
		// Need to write an asset bank for the Demo. Need the functions which will turn eg ModelComponent to buffer components 
		//	- still got some todos on this - got to do the other data

		// TODAY (MON) 
		// - Then collision with the asteroids (or a seperate obj)
		// - Do asteroids want rigidbody based movement? leave the transform based movement commented out though for an example
		// audio - keep it basic. move the listner with the player and have the asteroids / player play a noise with key
		// ui / text updating - keep it basic. a like 'asteroids spawned count' or something idk. keys pressed count maybe. a click button?
		// - then, done???

		// also, when getting closer to finishing ask sam what could be causing the slow speed on the render thread / fps
		// perhaps he can sit down with me and go through the code / look at what could be slowing it down?

		// so on the bad side... a lot of todos..
		// but on the good side - im glad we are doing this now. come on ben - you got this!!
} // namespace ecs::DemoModule

#endif // ifdef INCLUDE_DEMO

/*
* How to use an entity property list + create an entity template:
* 
	void SpawnAsteroid(nabi::Context& context, dx::XMFLOAT3 const& spawnPosition)
	{
		nabi::ECS::EntityCreator::EntityCreationSettings asteriodCreationSettings = {};
		nabi::ECS::EntityPropertyList propertyList;

		std::string const spawnPositionAsString = nabi::Reflection::StringConverter::ToString<dx::XMFLOAT3>(spawnPosition);
		propertyList.AddProperty("TransformComponent", "Position", spawnPositionAsString);

		asteriodCreationSettings.m_EntityName = "Asteroid";
		asteriodCreationSettings.m_EntityTemplateName = "AsteroidEntityTemplate";
		asteriodCreationSettings.m_EntityOverriddenProperties = &propertyList;

		context.m_EntityCreator->CreateEntity(&asteriodCreationSettings);
	}
*/
