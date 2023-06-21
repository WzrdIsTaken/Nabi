#pragma once
#include "EngineCore.h"

#include "DebugUtils.h"

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
		/// <summary>
		/// Checks if m_FixedDeltaTimeAccumulator >= c_FixedTimeStep. If this is true, then FixedUpdate should be called.
		/// Otherwise, it shouldn't
		/// </summary>
		/// <returns>Whether to run the physics simulation</returns>
		inline bool RunSimulation() const NABI_NOEXCEPT { return m_RunSimulation; };

		// Getters for all the cool time stuff this class holds
		inline Interval GetDeltaTime() const NABI_NOEXCEPT 
		{
			return m_DeltaTime;
		};
#ifndef USE_DEBUG_UTILS
		constexpr
#endif // ifndef USE_DEBUG_UTILS
		inline Interval GetFixedDeltaTime() const NABI_NOEXCEPT
		{
			ASSERT(RunSimulation(), "Getting FixedDeltaTime but RunSimulation is false");
			return c_FixedTimeStep;
		};
		Interval GetStartUpTime() const NABI_NOEXCEPT;

#ifdef USE_DEBUG_UTILS
		inline void ForceRunSimulationState(bool runSimulation) NABI_NOEXCEPT { m_RunSimulation = runSimulation; };
#endif // ifdef USE_DEBUG_UTILS
		
	private:
		static Interval constexpr c_MaxDeltaTime = 0.1;
		static Interval constexpr c_FixedTimeStep = 1.0 / 60.0; // 60 fps

		TimePoint m_CurrentTime;
		TimePoint m_PreviousTime;
		TimePoint m_StartUpTime;

		Duration m_DeltaTimeDuration;
		Interval m_DeltaTime;

		Interval m_FixedDeltaTimeAccumulator;
		bool m_RunSimulation;
	};
} // namespace nabi
