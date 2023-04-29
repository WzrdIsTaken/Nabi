#include "EngineCore.h"

#include "SystemBase.h"

#include "Context.h"

namespace nabi::ECS
{
	SystemBase::SystemBase(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId) NABI_NOEXCEPT
		: m_Context(context)
		, c_SystemId(systemId)
		, c_SystemGroupId(systemGroupId)
	{
	}
} // namespace nabi::ECS
