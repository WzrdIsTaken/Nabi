#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\ECS\Components\AsteroidComponent.h"

namespace ecs
{
	REFLECT_COMPONENT_BEGIN_DEFAULT(AsteroidComponent)
	REFLECT_COMPONENT_END(AsteroidComponent)
} // namespace ecs

#endif // ifdef INCLUDE_DEMO
