#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Modules\DemoModule.h"

#include "CoreModules\EntityModule.h"
#include "DirectXUtils.h"
#include "EntityPropertyList.h"

#include "Demo\ECS\SingletonComponents\DemoPropertiesComponent.h"

namespace ecs::DemoModule
{
	namespace
	{
		void SpawnAsteroid(nabi::Context& context, dx::XMFLOAT3 const& spawnPosition)
		{
			nabi::ECS::EntityCreator::EntityCreationSettings asteriodCreationSettings = {};
			nabi::ECS::EntityPropertyList propertyList;
			propertyList.AddProperty("TransformComponent", "Position", ""); // spawnPosition

			asteriodCreationSettings.m_EntityName = "Asteroid";
			asteriodCreationSettings.m_EntityTemplateName = "AsteroidEntityTemplate";
			// add the property list

			context.m_EntityCreator->CreateEntity(&asteriodCreationSettings);
		}
	}

	void StartDemo(nabi::Context& context)
	{
		// There is defo a way of writing the logic in this function better, but im very tired as its only a demo...
		// i think i can improve it with lambdas? do this. get it working first though

		unsigned int constexpr axisCount = 6u; // +/-x, +/-y, +/-z

		auto const& demoProperties = EntityModule::GetSingletonComponent<SComp::DemoPropertiesComponent>(context);
		float const asteroidSpacing = demoProperties.m_AsteriodSpawnRadius / static_cast<float>(demoProperties.m_AsteriodCount);
		unsigned int const asteroidsPerAxis = demoProperties.m_AsteriodCount / axisCount;

		dx::XMFLOAT3 asteroidSpawnPosition = nabi::DirectXUtils::c_Float3Zero;
		for (auto i = 0u; i < axisCount; ++i)
		{
			auto const axis = i;

			for (auto j = 1u; j < asteroidsPerAxis + 1u; ++j)
			{
				auto const asteroidSpawnMultiplier = static_cast<float>(j);
				float const asteroidSpawnAxisAdjustment = asteroidSpacing * asteroidSpawnMultiplier;

				switch (axis)
				{
				case 0u: // +x
					asteroidSpawnPosition.x += asteroidSpawnAxisAdjustment;
					break;
				case 1u: // -x
					asteroidSpawnPosition.x -= asteroidSpawnAxisAdjustment;
					break;
				case 2u: // +y
					asteroidSpawnPosition.y += asteroidSpawnAxisAdjustment;
					break;
				case 3u: // -y
					asteroidSpawnPosition.y -= asteroidSpawnAxisAdjustment;
					break;
				case 4u: // +z
					asteroidSpawnPosition.z += asteroidSpawnAxisAdjustment;
					break;
				case 5u: // -z
					asteroidSpawnPosition.z -= asteroidSpawnAxisAdjustment;
					break;
				}

				SpawnAsteroid(context, asteroidSpawnPosition);
			}

			asteroidSpawnPosition = nabi::DirectXUtils::c_Float3Zero;
		}
	}
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
