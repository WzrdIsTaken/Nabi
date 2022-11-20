#pragma once

#include "../Libraries/entt/entt.h"

namespace entt
{
	// forward declares
	// rule of 6?
}

namespace nabi
{
	class NabiCore
	{
	public:
		virtual ~NabiCore() = default;

		virtual void Init() = 0;
		virtual void Run() = 0;

		entt::registry& GetRegistry();

	protected:
		entt::registry m_Registry;
	};
} // namespace nabi
