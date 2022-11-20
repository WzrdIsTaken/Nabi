#pragma once

namespace nabi::ECS
{
	/// <summary>
	/// Base class for Systems. The registery and ids are things all systems will need. But no virtual inheritance!
	/// Could also be useful if I ever need to group all systems
	/// </summary>
	class SystemBase
	{
	public:
		SystemBase(entt::registry& registery, entt::hashed_string const systemId, entt::hashed_string const systemGroupId) 
			: m_Registery(registery)
			, c_SystemId(systemId)
			, c_SystemGroupId(systemGroupId)
		{
		}

		entt::hashed_string const& GetSystemId() const
		{
			return c_SystemId;
		}

		entt::hashed_string const& GetSystemGroupId() const
		{
			return c_SystemGroupId;
		}

	private:
		SystemBase(SystemBase const&) = delete;
		SystemBase& operator = (SystemBase const&) = delete;

		entt::registry& m_Registery;

		entt::hashed_string const c_SystemId;
		entt::hashed_string const c_SystemGroupId;
	};
} // namespace nabi::ECS
