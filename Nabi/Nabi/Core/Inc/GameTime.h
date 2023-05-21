#pragma once
#include "EngineCore.h"

namespace nabi
{
	class GameTime final
	{
	public:
		typedef std::chrono::steady_clock Clock;
		typedef Clock::time_point TimePoint;
		typedef double Interval;
		typedef std::chrono::duration<Interval> Duration;

		GameTime() NABI_NOEXCEPT;

		/// <summary>
		/// Updates delta time and fixed delta time. Should be called every frame
		/// </summary>
		void Tick() NABI_NOEXCEPT;

		// Getters for all the cool time stuff this class holds
		Interval GetDeltaTime() const NABI_NOEXCEPT;
		Interval GetFixedDeltaTime() const NABI_NOEXCEPT;
		Interval GetStartUpTime() const NABI_NOEXCEPT;
		
	private:
		static Interval constexpr c_MaxDeltaTime = 0.1;
		static Interval constexpr c_FixedTimeStep = 1.0 / 60.0; // 60 fps

		TimePoint m_CurrentTime;
		TimePoint m_PreviousTime;
		TimePoint m_StartUpTime;

		Duration m_DeltaTimeDuration;
		Interval m_DeltaTime;

		Interval m_FixedDeltaTimeAccumulator;
		Interval m_FixedDeltaTime;
	};
} // namespace nabi
