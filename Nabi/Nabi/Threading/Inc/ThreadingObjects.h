#pragma once
#include "EngineCore.h"

#include <synchapi.h>

#include "thread_pool.h"

namespace nabi::Threading
{
	struct ThreadingObjects final
	{
	private:
		typedef dp::details::default_function_type FunctionType;
		typedef std::jthread ThreadType;
		typedef dp::thread_pool<FunctionType, ThreadType> DefaultThreadPool;

		// a wip soontm idea
		//typedef dp::thread_safe_queue<FunctionType> DpQueueType;
		//typedef thread_safe_priority_queue<FunctionType> PriorityQueueType; 
		//typedef dp::thread_pool<FunctionType, ThreadType, PriorityQueueType> PriorityThreadPool;

	public:
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
