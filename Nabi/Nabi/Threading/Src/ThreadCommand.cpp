#include "EngineCore.h"

#include "ThreadCommand.h"

#include "InitSettings.h"

namespace nabi::Threading
{
	// --- Thread Command ---

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
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, GetTaskStatistics(), LOG_END);

		m_ThreadingObjects.m_ThreadPool.reset();
		DeleteCriticalSection(&m_ThreadingObjects.m_CriticalSection);
	}

	// --- Task Statistics ---
#ifdef USE_DEBUG_UTILS
	ThreadCommand::TaskStatistics::TaskStatistics()
		: m_TaskDurationStats
		{
			{ TaskDuration::Lifetime, { "lifetime", 0u } },
			{ TaskDuration::Long,     { "long",     0u } },
			{ TaskDuration::Medium,   { "medium",   0u } },
			{ TaskDuration::Short,    { "short",    0u } }
		}
		, m_TaskPriorityStats
		{
			{ TaskPriority::Critical, { "critical", 0u } },
			{ TaskPriority::High,     { "high",     0u } },
			{ TaskPriority::Medium,   { "medium",   0u } },
			{ TaskPriority::Low,      { "low",      0u } }
		}
	{
	}

	void ThreadCommand::TaskStatistics::NewTaskEnqueued(std::string const& action, std::string const& taskName, 
		TaskDuration const taskDuration, TaskPriority const taskPriority)
	{
		if (CheckTaskEnumIsValid(taskName, taskDuration, m_TaskDurationStats) && 
			CheckTaskEnumIsValid(taskName, taskPriority, m_TaskPriorityStats))
		{
			LogTaskEnqueueMessage(action, taskName, taskDuration, taskPriority);
			UpdateTaskStatistics(taskDuration, taskPriority);
		}
	}

	void ThreadCommand::TaskStatistics::LogTaskEnqueueMessage(std::string const& action, std::string const& taskName, 
		TaskDuration const taskDuration, TaskPriority const taskPriority) const
	{
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, action << " a "             <<
			m_TaskDurationStats.at(taskDuration).m_TaskName    << " length, "       <<
			m_TaskPriorityStats.at(taskPriority).m_TaskName    << " priority task " <<
			WRAP(taskName, "\""), LOG_END);
	}

	void ThreadCommand::TaskStatistics::UpdateTaskStatistics(TaskDuration const taskDuration, TaskPriority const taskPriority) NABI_NOEXCEPT
	{
		++m_TaskDurationStats.at(taskDuration).m_StartedCount;
		++m_TaskPriorityStats.at(taskPriority).m_StartedCount;
	}

	std::string ThreadCommand::TaskStatistics::GetStatistics() const NABI_NOEXCEPT
	{
		auto GetStatisticsHelper =
			[](std::ostringstream& stream, std::string const& statistic, auto const& statisticsMap)
			{
				constexpr size_t longestTaskName = 8u;

				stream << "Tasks started by " << statistic << ":" << NEWLINE;
				for (auto const& [stat, info] : statisticsMap)
				{
					stream << info.m_TaskName << SPACE(8 - info.m_TaskName.length()) << " - " << info.m_StartedCount << NEWLINE;
				}
			};

		std::ostringstream statistics;
		statistics << "Application Threading Statistics:" << NEWLINE;

		GetStatisticsHelper(statistics, "duration", m_TaskDurationStats);
		GetStatisticsHelper(statistics, "priority", m_TaskPriorityStats);
		
		std::string result = statistics.str();
		if (!result.empty())
		{
			// Removes the final NEWLINE character
			result.pop_back();
		}
		return result;
	}
#endif // ifdef USE_DEBUG_UTILS
} // namespace nabi::Threading
