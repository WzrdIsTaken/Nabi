#include "Demo\DemoCore.h"

#ifdef INCLUDE_DEMO

#include "Demo\Core\Demo.h"

#include "Demo\ECS\Modules\DemoModule.h"

namespace core
{
	int Demo::Init()
	{
		int const result = nabi::NabiCore::Init();
		ecs::DemoModule::StartDemo(m_Context);

		return result;
	}
} // namespace core

#endif // ifdef INCLUDE_DEMO
