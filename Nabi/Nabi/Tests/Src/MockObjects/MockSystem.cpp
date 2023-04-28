#include "Core.h"

#include "MockObjects/MockSystem.h"

#ifdef RUN_TESTS

namespace nabitest::ECS
{
	REFLECT_SYSTEM_BEGIN(MockSystem, "MockSystem")
		REFLECT_SYSTEM_PROPERTY(MockSystem::m_MockSystemData, "MockSystemData")
	RELFECT_SYSTEM_END(MockSystem)

	MockSystem::MockSystem(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId)
		: SystemBase(context, systemId, systemGroupId)
		, m_MockSystemData(0)
	{
		REGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(MockSystem)
		REGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(MockSystem)
	}

	MockSystem::~MockSystem()
	{
		UNREGISTER_SYSTEM_UPDATE_EVENT_SUBSCRIBER(MockSystem)
		UNREGISTER_SYSTEM_RENDER_EVENT_SUBSCRIBER(MockSystem)
	}

	void MockSystem::Update(/*game time*/) const
	{
		int some_var_so_i_can_easily_put_a_breakpoint_in_here = 0;
		++some_var_so_i_can_easily_put_a_breakpoint_in_here;
	}

	void MockSystem::Render() const
	{
		int some_other_var_so_i_can_easily_put_a_breakpoint_in_here = 0;
		--some_other_var_so_i_can_easily_put_a_breakpoint_in_here;
	}
} // namespace nabitest::ECS

#endif // #ifdef RUN_TESTS
