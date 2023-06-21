#pragma once
#include "EngineCore.h"

#include "DebugUtils.h"
#include "ThreadingObjects.h"

namespace nabi
{
	struct ThreadingSettings;
} // namespace nabi

namespace nabi::Threading
{
#define LIFETIME_TASK nabi::Threading::ThreadCommand::TaskDuration::Lifetime
#define LONG_TASK     nabi::Threading::ThreadCommand::TaskDuration::Long
#define MEDIUM_TASK   nabi::Threading::ThreadCommand::TaskDuration::Medium
#define SHORT_TASK    nabi::Threading::ThreadCommand::TaskDuration::Short

#define CRITICAL_PRIORITY nabi::Threading::ThreadCommand::TaskPriority::Critical
#define HIGH_PRIORITY     nabi::Threading::ThreadCommand::TaskPriority::High
#define MEDIUM_PRIORITY   nabi::Threading::ThreadCommand::TaskPriority::Medium
#define LOW_PRIORITY      nabi::Threading::ThreadCommand::TaskPriority::Low

	/// <summary>
	/// Manages a thread pool submits tasks to a priority queue 
	/// Note - it is possible that the thread pool will be null on shutdown and so a task won't be queued. Hence 'if (m_ThreadingObjects.m_ThreadPool)'.
	/// However, this isn't a problem. If the log message appears while the program is running though... then yeah it might well be a problem :p
	/// </summary>
	class ThreadCommand final
	{
	public:
		enum class TaskDuration : unsigned int
		{
			Lifetime = 1u,
			Long     = 2u,
			Medium   = 3u,
			Short    = 4u,
			ENUM_COUNT
		};
		enum class TaskPriority : unsigned int
		{
			Critical = 1u,
			High     = 2u,
			Medium   = 3u,
			Low      = 4u,
			ENUM_COUNT
		};

		ThreadCommand(ThreadingObjects& threadingObjects, ThreadingSettings const& threadingSettings) NABI_NOEXCEPT;
		~ThreadCommand();

		template <typename Function, typename... Args, typename ReturnType = std::invoke_result_t<Function&&, Args &&...>> requires std::invocable<Function, Args...>
		[[nodiscard]] inline std::future<ReturnType> EnqueueTask(std::string const& taskName, TaskDuration const taskDuration, TaskPriority const taskPriority, 
			Function function, Args... args) const NABI_NOEXCEPT
		{
			if (m_ThreadingObjects.m_ThreadPool)
			{
				ASSERT_CODE(LogTaskEnqueueMessage("Enqueued", taskName, taskDuration, taskPriority));
				return m_ThreadingObjects.m_ThreadPool->enqueue(std::forward<Function>(function), std::forward<Args>(args)...);
			}
			else
			{
				LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, "Trying to enqueue a task but the thread pool is null", LOG_END);
				return {};
			}
		}
		template <typename Function, typename... Args> requires std::invocable<Function, Args...> && std::is_same_v<void, std::invoke_result_t<Function&&, Args &&...>>
		[[nodiscard]] inline void EnqueueTaskDetach(std::string const& taskName, TaskDuration const taskDuration, TaskPriority const taskPriority, 
			Function function, Args... args) const NABI_NOEXCEPT
		{
			if (m_ThreadingObjects.m_ThreadPool)
			{
				ASSERT_CODE(LogTaskEnqueueMessage("Enqueued and detached", taskName, taskDuration, taskPriority));
				m_ThreadingObjects.m_ThreadPool->enqueue_detach(std::forward<Function>(function), std::forward<Args>(args)...);
			}
			else
			{
				LOG(LOG_PREP, LOG_WARN, LOG_CATEGORY_THREADING, "Trying to enqueue and detach a task but the thread pool is null", LOG_END);
			}
		}

		[[nodiscard]] inline CRITICAL_SECTION& GetCriticalSection() const NABI_NOEXCEPT
		{
			return m_ThreadingObjects.m_CriticalSection;
		}
		[[nodiscard]] inline size_t GetThreadPoolSize() const NABI_NOEXCEPT 
		{ 
			ASSERT_FATAL(m_ThreadingObjects.m_ThreadPool, "Trying to get the thread pool's size but the thread pool is null");
			return m_ThreadingObjects.m_ThreadPool->size(); 
		};

	private:
		DELETE_COPY_MOVE_CONSTRUCTORS(ThreadCommand)

#ifdef USE_DEBUG_UTILS
		void LogTaskEnqueueMessage(std::string const& action, std::string const& taskName, 
			TaskDuration const taskDuration, TaskPriority const taskPriority) const NABI_NOEXCEPT;
#endif // ifdef USE_DEBUG_UTILS

		ThreadingObjects& m_ThreadingObjects;
	};
} // namespace nabi::Threading
