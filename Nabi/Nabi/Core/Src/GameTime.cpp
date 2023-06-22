#include "EngineCore.h"

#include "GameTime.h"

#include "MathUtils.h"

namespace nabi
{
	// --- Game Time ---

	GameTime::GameTime() NABI_NOEXCEPT
		// Reference times
		: m_StartUpTime(Clock::now())
		, m_LastSimulationTick(m_StartUpTime)

		// Trackers
		, m_DeltaTimeTracker{ m_StartUpTime }
		, m_FrameTimeTracker{ m_StartUpTime }

		// Fixed delta time
		, m_FixedDeltaTimeAccumulator(0.0)
		, m_RunSimulation(false)
	{
	}

	void GameTime::Tick() NABI_NOEXCEPT
	{
		// Delta time
		m_DeltaTimeTracker.Tick();

		ASSERT_CODE
		(
			bool const deltaTimeIsMax = nabi::MathUtils::Approximity(m_DeltaTimeTracker.GetTime(), c_MaxDeltaTime);
			CONDITIONAL_LOG(deltaTimeIsMax, LOG_PREP, LOG_WARN, LOG_CATEGORY_CORE, 
				"Delta time is being capped to its maximum value (" << c_MaxDeltaTime << ")!", LOG_END);
		)

		// Fixed delta time
		m_FixedDeltaTimeAccumulator += m_DeltaTimeTracker.GetTime();
		if (m_FixedDeltaTimeAccumulator >= c_FixedTimeStep)
		{
			if (!m_RunSimulation)
			{
				m_LastSimulationTick = Clock::now();
				m_RunSimulation = true;
			}

			m_FixedDeltaTimeAccumulator -= c_FixedTimeStep;
		}
		else
		{
			if (m_RunSimulation)
			{
				m_RunSimulation = false;
			}
		}
	}

	void GameTime::TickFrame() NABI_NOEXCEPT
	{
		m_FrameTimeTracker.Tick();
	}

	// --- Time Tracker ---
	// Could it be useful to pass in the current time to Tick()?
	// Also, is it bad we are using c_MaxDeltaTime for FrameTime as well? (as of now, FrameTime isn't used though - only fps for performace tracking)

	GameTime::TimeTracker::TimeTracker(TimePoint const& currentTime) NABI_NOEXCEPT
		// Tick/Frame time
		: m_CurrentTime(currentTime)
		, m_PreviousTime(m_CurrentTime)
		, m_ElapsedTime{ 0.0 }
		, m_Time(0.0)

		// TPS / FPS
		, m_TickElapsedTime{ 0.0 }
		, m_TickCount(0u)
		, m_Ticks(0u)
	{
	}

	void GameTime::TimeTracker::Tick() NABI_NOEXCEPT
	{
		UpdateTime();
		UpdateTicks();
	}

	void GameTime::TimeTracker::UpdateTime() NABI_NOEXCEPT
	{
		m_PreviousTime = m_CurrentTime;
		m_CurrentTime = Clock::now();

	    m_ElapsedTime = m_CurrentTime - m_PreviousTime;
		m_Time = std::fmin(m_ElapsedTime.count(), c_MaxDeltaTime);
	}

	void GameTime::TimeTracker::UpdateTicks() NABI_NOEXCEPT
	{
		++m_TickCount;
		m_TickElapsedTime += m_CurrentTime - m_PreviousTime;

		Interval const elapsedTime = m_TickElapsedTime.count();
		if (elapsedTime >= 1.0)
		{
			m_Ticks = static_cast<uint32_t>(m_TickCount / elapsedTime);
			m_TickCount = 0u;
			m_TickElapsedTime = Duration(0.0);
		}
	}
} // namespace nabi
