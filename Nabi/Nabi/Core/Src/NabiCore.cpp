#include "../EngineCore.h"

#include "../Inc/NabiCore.h"

namespace nabi
{
	inline entt::registry& NabiCore::GetRegistry() NABI_NOEXCEPT
	{
		return m_Registry;
	}
} // namespace nabi
