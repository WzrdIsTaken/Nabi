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

	class ThreadCommand final
	{
	public:
		enum class TaskDuration : int
		{
			Lifetime,
			Long,
			Medium,
			Short,
			ENUM_COUNT
		};
		enum class TaskPriority : int
		{
			Critical,
			High,
			Medium,
			Low,
			ENUM_COUNT
		};

		ThreadCommand(ThreadingObjects& threadingObjects, ThreadingSettings const& threadingSettings) NABI_NOEXCEPT;
		~ThreadCommand();

		template <typename Function, typename... Args, typename ReturnType = std::invoke_result_t<Function&&, Args &&...>> requires std::invocable<Function, Args...>
		[[nodiscard]] inline std::future<ReturnType> EnqueueTask(std::string const& taskName, TaskDuration const /*taskDuration*/, TaskPriority const /*taskPriority*/, 
			Function function, Args... args) const NABI_NOEXCEPT
		{
			ASSERT_FATAL(m_ThreadingObjects.m_ThreadPool, "Trying to enqueue a task but the thread pool is null");
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, "Enqueued a task " << WRAP(taskName, "'"), LOG_END);

			return m_ThreadingObjects.m_ThreadPool->enqueue(std::forward<Function>(function), std::forward<Args>(args)...);
		}
		template <typename Function, typename... Args> requires std::invocable<Function, Args...> && std::is_same_v<void, std::invoke_result_t<Function&&, Args &&...>>
		[[nodiscard]] inline void EnqueueTaskDetach(std::string const& taskName, TaskDuration const /*taskDuration*/, TaskPriority const /*taskPriority*/,
			Function function, Args... args) const NABI_NOEXCEPT
		{
			ASSERT_FATAL(m_ThreadingObjects.m_ThreadPool, "Trying to enqueue a task but the thread pool is null");
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_THREADING, "Enqueued and detached a task " << WRAP(taskName, "'"), LOG_END);

			m_ThreadingObjects.m_ThreadPool->enqueue_detach(std::forward<Function>(function), std::forward<Args>(args)...);
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

		ThreadingObjects& m_ThreadingObjects;
	};
} // namespace nabi::Threading
