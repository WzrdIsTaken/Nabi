#pragma once
#include "EngineCore.h"

#include "entt.h"

namespace nabi
{
	class NabiCore abstract
	{
	public:
		NabiCore() NABI_NOEXCEPT = default;
		virtual ~NabiCore() = default;

		virtual void Init() = 0; // noexcept?
		virtual void Run() = 0;  // " "

		[[nodiscard]] inline entt::registry& GetRegistry() NABI_NOEXCEPT;

	protected:
		entt::registry m_Registry;

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(NabiCore)
	};
} // namespace nabi
