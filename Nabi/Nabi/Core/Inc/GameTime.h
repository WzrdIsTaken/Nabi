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
		typedef uint32_t Ticks;

		GameTime() NABI_NOEXCEPT;

		/// <summary>
		/// Updates delta time and fixed delta time. Should be called every update tick
		/// </summary>
		void Tick() NABI_NOEXCEPT;
		/// <summary>
		/// Updates fps. Should be called every render tick
		/// </summary>
		void TickFrame() NABI_NOEXCEPT;

		/// <summary>
		/// Checks if m_FixedDeltaTimeAccumulator >= c_FixedTimeStep. If this is true, then FixedUpdate should be called.
		/// Otherwise, it shouldn't
		/// </summary>
		/// <returns>Whether to run the physics simulation</returns>
		[[nodiscard]] inline bool RunSimulation() const NABI_NOEXCEPT { return m_RunSimulation; };

		// Getters for all the cool time stuff this class has..:
		[[nodiscard]] inline Interval GetDeltaTime() const NABI_NOEXCEPT
		{
			return m_DeltaTimeTracker.GetTime();
		};
		[[nodiscard]] inline Interval GetFrameTime() const NABI_NOEXCEPT
		{
			return m_FrameTimeTracker.GetTime();
		}
		[[nodiscard]] /*constexpr*/ inline Interval GetFixedDeltaTime() const NABI_NOEXCEPT
		{
			ASSERT(RunSimulation(), "Getting FixedDeltaTime but RunSimulation is false");
			return c_FixedTimeStep;
		};

		[[nodiscard]] inline Ticks GetTps() const NABI_NOEXCEPT { return m_DeltaTimeTracker.GetTicks(); };
		[[nodiscard]] inline Ticks GetFps() const NABI_NOEXCEPT { return m_FrameTimeTracker.GetTicks(); };
		[[nodiscard]] inline Interval GetStartUpTime() const NABI_NOEXCEPT        { return static_cast<Interval>((Clock::now() - m_StartUpTime).count()); };
		[[nodiscard]] inline Interval GetLastSimulationTick() const NABI_NOEXCEPT { return static_cast<Interval>((Clock::now() - m_LastSimulationTick).count()); };

#ifdef USE_DEBUG_UTILS
		inline void ForceRunSimulationState(bool runSimulation) NABI_NOEXCEPT { m_RunSimulation = runSimulation; };
#endif // ifdef USE_DEBUG_UTILS
		
	private:
		static Interval constexpr c_MaxDeltaTime  = 0.1;
		static Interval constexpr c_FixedTimeStep = 1.0 / 60.0; // 60 fps

		class TimeTracker final
		{
		public:
			TimeTracker(TimePoint const& currentTime) NABI_NOEXCEPT;
			void Tick() NABI_NOEXCEPT;

			[[nodiscard]] inline Interval GetTime() const NABI_NOEXCEPT { return m_Time; };
			[[nodiscard]] inline Ticks GetTicks() const NABI_NOEXCEPT { return m_Ticks; };

		private:
			void UpdateTime() NABI_NOEXCEPT;
			void UpdateTicks() NABI_NOEXCEPT;

			TimePoint m_CurrentTime;
			TimePoint m_PreviousTime;

			Duration  m_ElapsedTime;
			Interval  m_Time;

			Duration m_TickElapsedTime;
			Ticks m_TickCount;
			Ticks m_Ticks;
		};

		TimePoint m_StartUpTime;
		TimePoint m_LastSimulationTick;

		TimeTracker m_DeltaTimeTracker;
		TimeTracker m_FrameTimeTracker;

		Interval m_FixedDeltaTimeAccumulator;
		bool m_RunSimulation;
	};
} // namespace nabi
