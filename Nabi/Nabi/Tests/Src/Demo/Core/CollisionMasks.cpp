#include "Demo\DemoCore.h"

#include "Demo\Core\CollisionMasks.h"

namespace core
{
	REFLECT_ENUM_UNDERLYING_BEGIN_DEFAULT(CollisionMask)
		REFLECT_ENUM_VALUE(CollisionMask::Object, "Object")
		REFLECT_ENUM_VALUE(CollisionMask::Player, "Player")
	REFLECT_ENUM_END(CollisionMask)
} // namespace core
