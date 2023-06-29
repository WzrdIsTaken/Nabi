#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\SingletonComponents\DemoPropertiesComponent.h"

namespace ecs::SComp
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(DemoPropertiesComponent)
		REFLECT_COMPONENT_PROPERTY(DemoPropertiesComponent::m_AsteroidsPerAxis, "AsteroidsPerAxis")
		REFLECT_COMPONENT_PROPERTY(DemoPropertiesComponent::m_AsteroidSpacing, "AsteroidSpacing")
	REFLECT_COMPONENT_END(DemoPropertiesComponent)
} // namespace ecs::SComp

#endif // ifdef INCLUDE_DEMO
