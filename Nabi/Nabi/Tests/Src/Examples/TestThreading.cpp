#include "TestCore.h"

#include "Examples\TestThreading.h"

#include "CoreModules\InputModule.h"
#include "CoreSystems\InputSystem.h"
#include "LockCriticalSection.h"

#ifdef RUN_TESTS

namespace nabitest::Examples
{
	long long constexpr c_ThreadSleepTime = 2000;

	TestThreading::TestThreading(nabi::Context& context)
		: m_Context(context)
	{
	}

	TestThreading::~TestThreading()
	{
	}

	bool TestThreading::Init()
	{
		m_InputSystem = std::make_unique<ecs::InputSystem>(m_Context, "Input"_hs, "NabiThreadingTestSystems"_hs);
		return true;
	}

	bool TestThreading::Update()
	{
		using namespace nabi::Input;
		InputState const wKeyState = ecs::InputModule::GetKeyboardKey(m_Context, InputCode::Key_W);

		if (wKeyState == InputState::Pressed)
		{
			/*
			m_Context.m_ThreadCommand->EnqueueTaskDetach("VoidFunc", SHORT_TASK, HIGH_PRIORITY,
				[&]() -> void
				{
					VoidFunc();
				});
			*/

			/*
			std::future<int> future = m_Context.m_ThreadCommand->EnqueueTask("IntFunc", SHORT_TASK, HIGH_PRIORITY,
				[&]() -> int
				{
					return IntFunc();
				});

			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, "Other work here", LOG_END);
			int result = future.get();
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, result, LOG_END);
			*/

			/*
			std::future<float> future = m_Context.m_ThreadCommand->EnqueueTask("ParamFunc", SHORT_TASK, HIGH_PRIORITY,
				[&](float const f) -> float
				{
					return ParamFunc(f);
				}, 5.9f);

			float result = future.get();
			LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, result, LOG_END);
			*/

			/*
			{
				LOCK_CRITICAL_SECTION(m_Context)

				int i = 0;
				i++;
			}
			*/

			m_Context.m_ThreadCommand->EnqueueTaskDetach("ThreadInAThread", SHORT_TASK, HIGH_PRIORITY,
				[&]() -> void
				{
					ThreadInsideAThreadFunc();
				});

			// Next up - review written code, test the lock critical section stuff DONE
			// try putting the render thread etc on another thread? DONE
			// task priorities? can i just modiify the lib code to use a prioty deque? [not worth the effort]

			// look into ENTT_USE_ATOMIC / entts multithreading needs <---THIS

			// TODO need to check we have enough threads for this and handle if we dont?? DONE
			// Saw the "getting fixed dt but run sim is false" assert while idiling. remove this assert and make it a log warn? <---THIS
			//	- if did this also have to probs bump the slow test log to error and the main test log filter to error as well..
			// then can also remove the ifdef debug utils random stuff in collision tests <---THIS

			// test parsing xml data on another thread <---THIS
			// test loading data at runtime (eg new rendering data) <---THIS
			// always maintain non core functionality multithread capabilties so we can roll back if needed YE
			// need a task thread? NO i dont think so

			// WHY did we have to change the speed. deeper underlying problem? <---THIS
		}
		return true;
	}

	bool TestThreading::Render()
	{
		return false;
	}

	void TestThreading::VoidFunc() const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(c_ThreadSleepTime));
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, "Hello!", LOG_END);
	}

	int TestThreading::IntFunc() const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(c_ThreadSleepTime));
		return 21;
	}

	float TestThreading::ParamFunc(float const f) const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(c_ThreadSleepTime));
		return f;
	}

	void TestThreading::ThreadInsideAThreadFunc() const
	{
		std::future<int> result = m_Context.m_ThreadCommand->EnqueueTask("SubThread", SHORT_TASK, MEDIUM_PRIORITY,
			[&]() -> int 
			{
				return IntFunc();
			});

		int value = result.get();
		++value;
		LOG(LOG_PREP, LOG_INFO, LOG_CATEGORY_TEST, value, LOG_END);
	}
} // namespace nabitest::Examples

#endif // ifdef RUN_TESTS
