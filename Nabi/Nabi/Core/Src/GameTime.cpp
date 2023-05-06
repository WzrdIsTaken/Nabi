#include "EngineCore.h"

#include "GameTime.h"

#include "DebugUtils.h"
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
		, m_FixedDeltaTime(c_FixedTimeStep)
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
			using namespace nabi::Utils;
			bool const deltaTimeIsMax = MathUtils::Approximity(m_DeltaTime, c_MaxDeltaTime);
			CONDITIONAL_LOG(deltaTimeIsMax, LOG_PREP, LOG_WARN, LOG_CATEGORY_CORE <<
				"Delta time is being capped to its maximum value (" << c_MaxDeltaTime << ")!" << ENDLINE);
		)

		// Fixed delta time
		// soontm? something to do with an accumulator...

		m_PreviousTime = m_CurrentTime;
	}

	GameTime::Interval GameTime::GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	GameTime::Interval GameTime::GetFixedDeltaTime() const
	{
		FUNCTION_NOT_IMPLEMENTED
		return m_FixedDeltaTime;
	}

	GameTime::Interval GameTime::GetStartUpTime() const
	{
		Duration const timeSinceStartUp = m_CurrentTime - m_StartUpTime;
		Interval const timeSinceStartUpAsInterval = timeSinceStartUp.count();

		return timeSinceStartUpAsInterval;
	}
} // namespace nabi
