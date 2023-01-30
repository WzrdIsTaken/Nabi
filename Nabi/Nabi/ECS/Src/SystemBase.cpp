#include "EngineCore.h"

#include "SystemBase.h"

namespace nabi::ECS
{
	SystemBase::SystemBase(entt::registry& registery, entt::hashed_string const systemId, entt::hashed_string const systemGroupId) NABI_NOEXCEPT
		: m_Registery(registery)
		, c_SystemId(systemId)
		, c_SystemGroupId(systemGroupId)
	{
	}

	entt::hashed_string const& SystemBase::GetSystemId() const NABI_NOEXCEPT
	{
		return c_SystemId;
	}

	entt::hashed_string const& SystemBase::GetSystemGroupId() const NABI_NOEXCEPT
	{
		return c_SystemGroupId;
	}
} // namespace nabi::ECS
