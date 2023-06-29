#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Modules\DemoModule.h"

#include "CoreModules\EntityModule.h"
#include "DirectXUtils.h"
#include "EntityPropertyList.h"

#include "Demo\ECS\SingletonComponents\DemoPropertiesComponent.h"
#include "BaseTypeReflection.h"

namespace ecs::DemoModule
{
	namespace
	{
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
	}

	void StartDemo(nabi::Context& context)
	{
		int constexpr positiveMultiplier =  1;
		int constexpr negativeMultiplier = -1;

		auto const& demoProperties = EntityModule::GetSingletonComponent<SComp::DemoPropertiesComponent>(context);
		dx::XMFLOAT3 asteroidSpawnPosition = nabi::DirectXUtils::c_Float3Zero;

		auto SpawnAsteroidsOnAxis =
			[&context, &asteroidSpawnPosition,
				asteroidCount = demoProperties.m_AsteroidsPerAxis, 
				asteroidSpacing = demoProperties.m_AsteroidSpacing]
					(float& axis, int const axisMultiplier) -> void
			{
				for (auto i = 1u; i < asteroidCount + 1u; ++i)
				{
					axis = (asteroidSpacing * static_cast<float>(i)) * static_cast<float>(axisMultiplier);
					SpawnAsteroid(context, asteroidSpawnPosition);
				}

				asteroidSpawnPosition = nabi::DirectXUtils::c_Float3Zero;
			};

		// There are 6 axis: +/-x, +/-y, +/-z

		// x
		SpawnAsteroidsOnAxis(asteroidSpawnPosition.x, positiveMultiplier); 
		SpawnAsteroidsOnAxis(asteroidSpawnPosition.x, negativeMultiplier);

		// y
		SpawnAsteroidsOnAxis(asteroidSpawnPosition.y, positiveMultiplier); 
		SpawnAsteroidsOnAxis(asteroidSpawnPosition.y, negativeMultiplier);

		// z
		SpawnAsteroidsOnAxis(asteroidSpawnPosition.z, positiveMultiplier); 
		SpawnAsteroidsOnAxis(asteroidSpawnPosition.z, negativeMultiplier);
	}
} // namespace ecs::DemoModule

#endif // ifdef INCLUDE_DEMO
