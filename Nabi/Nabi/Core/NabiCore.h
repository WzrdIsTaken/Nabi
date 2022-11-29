#pragma once

#include "../Libraries/entt/entt.h"

namespace nabi
{
	class NabiCore abstract
	{
	public:
		NabiCore() = default;
		virtual ~NabiCore() = default;

		virtual void Init() = 0;
		virtual void Run() = 0;

		entt::registry& GetRegistry();

	protected:
		entt::registry m_Registry;

	private:
		NabiCore(NabiCore const&) = delete;
		NabiCore& operator = (NabiCore const&) = delete;
	};
} // namespace nabi
