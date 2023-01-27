#pragma once

#include "../../Core/EngineCore.h"

#include "../../Libraries/Inc/entt/entt.h"

namespace nabi::ECS
{
	/// <summary>
	/// Base class for Systems. The registery and ids are things all systems will need. But no virtual inheritance!
	/// Could also be useful if I ever need to group all systems
	/// </summary>
	class SystemBase
	{
	public:
		SystemBase(entt::registry& registery, entt::hashed_string const systemId, entt::hashed_string const systemGroupId) NABI_NOEXCEPT;

		[[nodiscard]] entt::hashed_string const& GetSystemId() const NABI_NOEXCEPT;
		[[nodiscard]] entt::hashed_string const& GetSystemGroupId() const NABI_NOEXCEPT;

	private:
		entt::registry& m_Registery;

		entt::hashed_string const c_SystemId;
		entt::hashed_string const c_SystemGroupId;
	};
} // namespace nabi::ECS
