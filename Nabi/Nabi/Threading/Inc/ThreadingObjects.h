#pragma once
#include "EngineCore.h"

#include <synchapi.h>

#include "thread_pool.h"

namespace nabi::Threading
{
	struct ThreadingObjects final
	{
		typedef dp::thread_pool<dp::details::default_function_type, std::jthread> DefaultThreadPool;
		typedef DefaultThreadPool ThreadPool;

		CRITICAL_SECTION m_CriticalSection;
		std::unique_ptr<ThreadPool> m_ThreadPool;
	};

	ThreadingObjects const c_ThreadingObjects
	{
		.m_CriticalSection = {},
		.m_ThreadPool = nullptr
	};
} // namespace nabi::Threading
