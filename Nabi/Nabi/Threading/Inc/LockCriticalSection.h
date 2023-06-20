#pragma once
#include "EngineCore.h"

#include <synchapi.h>

/*
* Make sure this is scoped inside a function or {}. Eg:
  {
 	LOCK_CRITICAL_SECTION
	~code
  }
*/
#define LOCK_CRITICAL_SECTION(context) nabi::Threading::LockCriticalSection lockCriticalSection(context.m_ThreadCommand->GetCriticalSection());

namespace nabi::Threading
{
	class LockCriticalSection final
	{
	public:
		LockCriticalSection(CRITICAL_SECTION& criticalSection) NABI_NOEXCEPT
			: m_CriticalSection(criticalSection)
		{
			EnterCriticalSection(&m_CriticalSection);
		}
		~LockCriticalSection()
		{
			LeaveCriticalSection(&m_CriticalSection);
		}

	private:
		CRITICAL_SECTION& m_CriticalSection;
	};
} // namespace nabi::Threading
