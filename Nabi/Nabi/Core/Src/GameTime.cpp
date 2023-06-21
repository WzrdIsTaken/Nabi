#include "EngineCore.h"

#include "GameTime.h"

#include "MathUtils.h"

namespace nabi
{
	GameTime::GameTime()
		// Clocks
		: m_CurrentTime(Clock::now())
		, m_PreviousTime(m_CurrentTime)
		, m_StartUpTime(m_CurrentTime)

		// Delta time
		, m_DeltaTimeDuration(0.0)
		, m_DeltaTime(0.0)

		// Fixed delta time
		, m_FixedDeltaTimeAccumulator(0.0)
		, m_RunSimulation(false)
	{
	}

	void GameTime::Tick()
	{
		m_CurrentTime = Clock::now();

		// Delta time
		m_DeltaTimeDuration = m_CurrentTime - m_PreviousTime;
		m_DeltaTime = std::fmin(m_DeltaTimeDuration.count(), c_MaxDeltaTime);

		ASSERT_CODE
		(
			bool const deltaTimeIsMax = nabi::MathUtils::Approximity(m_DeltaTime, c_MaxDeltaTime);
			CONDITIONAL_LOG(deltaTimeIsMax, LOG_PREP, LOG_WARN, LOG_CATEGORY_CORE, 
				"Delta time is being capped to its maximum value (" << c_MaxDeltaTime << ")!", LOG_END);
		)

		// Fixed delta time
		m_FixedDeltaTimeAccumulator += m_DeltaTime;
		if (m_FixedDeltaTimeAccumulator >= c_FixedTimeStep)
		{
			if (!m_RunSimulation)
			{
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

		m_PreviousTime = m_CurrentTime;
	}

	GameTime::Interval GameTime::GetStartUpTime() const
	{
		Duration const timeSinceStartUp = m_CurrentTime - m_StartUpTime;
		Interval const timeSinceStartUpAsInterval = timeSinceStartUp.count();

		return timeSinceStartUpAsInterval;
	}
} // namespace nabi
