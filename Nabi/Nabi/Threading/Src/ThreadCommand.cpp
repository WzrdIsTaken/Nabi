#include "EngineCore.h"

#include "ThreadCommand.h"

#include "InitSettings.h"

namespace nabi::Threading
{
	ThreadCommand::ThreadCommand(ThreadingObjects& threadingObjects, ThreadingSettings const& threadingSettings) NABI_NOEXCEPT
		: m_ThreadingObjects(threadingObjects)
#ifdef USE_DEBUG_UTILS
		, m_TaskStatistics{}
#endif // ifdef USE_DEBUG_UTILS
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

#ifdef USE_DEBUG_UTILS
	void ThreadCommand::TaskStatistics::UpdateTaskStatistics(std::string const& taskName, TaskDuration const taskDuration, TaskPriority const taskPriority) NABI_NOEXCEPT
	{
		if (m_TasksStartedByDuration.contains(taskDuration) && m_TasksStartedByPriority.contains(taskPriority))
		{
			++m_TasksStartedByDuration.at(taskDuration);
			++m_TasksStartedByPriority.at(taskPriority);
		}
		else
		{
			LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, "Trying to track a task " << WRAP(taskName, "\"") <<
				" with an unrecognised duration or priority", LOG_END);
		}
	}

	void ThreadCommand::LogTaskEnqueueMessage(std::string const& action, std::string const& taskName, 
		TaskDuration const taskDuration, TaskPriority const taskPriority) const NABI_NOEXCEPT
	{
		// It would be better to use Reflection::EnumConverter::EnumToString<TaskDuration>(taskDuration) here, 
		// but currently that functionality is not implemented and not worth doing for this debug use case

		static std::unordered_map<TaskDuration, std::string> const taskDurationToString =  {
			{ TaskDuration::Lifetime, "lifetime" }, { TaskDuration::Long,  "long"  },
			{ TaskDuration::Medium,   "medium"   }, { TaskDuration::Short, "short" },
		};
		static std::unordered_map<TaskPriority, std::string> const traskPriorityToString = {
			{ TaskPriority::Critical, "critical" }, { TaskPriority::High, "high"   },
			{ TaskPriority::Medium,   "medium"   }, { TaskPriority::Low,  "low"    },
		};

		if (taskDurationToString.contains(taskDuration) && traskPriorityToString.contains(taskPriority))
		{
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, action << " a " <<
				taskDurationToString .at(taskDuration) << " length, "       <<
				traskPriorityToString.at(taskPriority) << " priority task " <<
				WRAP(taskName, "\""), LOG_END);
		}
		else
		{
			LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, "Starting a task " << WRAP(taskName, "\"") << 
				" with an unrecognised duration or priority", LOG_END);
		}

		m_TaskStatistics.UpdateTaskStatistics(taskName, taskDuration, taskPriority);
	}
#endif // ifdef USE_DEBUG_UTILS
} // namespace nabi::Threading
