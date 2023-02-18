#pragma once
#include "EngineCore.h"

#include "entt.h"

// Forward declares
namespace nabi
{
	struct Context;
} // namespace nabi

namespace nabi::ECS
{
	/// <summary>
	/// Base class for Systems. The registery and ids are things all systems will need. But no virtual inheritance!
	/// Could also be useful if I ever need to group all systems
	/// </summary>
	class SystemBase
	{
	public:
		SystemBase(nabi::Context& context, entt::hashed_string const systemId, entt::hashed_string const systemGroupId) NABI_NOEXCEPT;

		[[nodiscard]] entt::hashed_string const& GetSystemId() const NABI_NOEXCEPT;
		[[nodiscard]] entt::hashed_string const& GetSystemGroupId() const NABI_NOEXCEPT;

	private:
		nabi::Context& m_Context;

		entt::hashed_string const c_SystemId;
		entt::hashed_string const c_SystemGroupId;
	};
} // namespace nabi::ECS
