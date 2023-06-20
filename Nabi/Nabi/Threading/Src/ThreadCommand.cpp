#include "EngineCore.h"

#include "ThreadCommand.h"

#include "InitSettings.h"

namespace nabi::Threading
{
	ThreadCommand::ThreadCommand(ThreadingObjects& threadingObjects, ThreadingSettings const& threadingSettings) NABI_NOEXCEPT
		: m_ThreadingObjects(threadingObjects)
	{
		unsigned int const maxHardwareThreads = std::thread::hardware_concurrency();
		unsigned int threadPoolSize = threadingSettings.m_ThreadPoolSize;
		if (threadPoolSize > maxHardwareThreads)
		{
			LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, 
				"Trying to create the thread pool with size " << threadPoolSize << " which is greater than std::thread::hardware_concurrency." <<
				"Limiting the number of threads to " << maxHardwareThreads, LOG_END);
			threadPoolSize = maxHardwareThreads;
		}

		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, "Creating the thread pool with " << threadPoolSize << " threads", LOG_END);
		m_ThreadingObjects.m_ThreadPool = std::make_unique<ThreadingObjects::ThreadPool>(threadPoolSize);

		InitializeCriticalSection(&m_ThreadingObjects.m_CriticalSection);
	}

	ThreadCommand::~ThreadCommand()
	{
		m_ThreadingObjects.m_ThreadPool.reset();
		DeleteCriticalSection(&m_ThreadingObjects.m_CriticalSection);
	}
} // namespace nabi::Threading
