#pragma once

#include "TypeUtils.h"

namespace nabi::Utils::MathUtils
{
	// --- Random Number Generations ---

	/*
	  It might be good to move away from rand() in the future? But for now its fine.
	  Perhaps https://prng.di.unimi.it/xoshiro256plusplus.c?
	*/

	/// <summary>
	/// Seeds random. Should only be called once on program startup
	/// </summary>
	void SeedRandom() NABI_NOEXCEPT;

	/// <summary>
	/// Generates a random number in the range of [0] - [max]
	/// </summary>
	/// <param name="max">- The maximum random number that will be generated</param>
	/// <returns>A random number</returns>
	[[nodiscard]] int GenerateRandomNumber(int const max) NABI_NOEXCEPT;

	/// <summary>
	/// Creates a random number in the range of [min] - [max]
	/// </summary>
	/// <param name="min">- The minimum random number that will be generated</param>
	/// <param name="max">- The maximum random number that will be generated</param>
	/// <returns>A random number</returns>
	[[nodiscard]] int GenerateRandomNumberInRange(int const min, int const max) NABI_NOEXCEPT;

	/// <summary>
	/// Returns true if the two values are approximity equal
	/// </summary>
	double constexpr c_dEpsilon = 0.00001;
	float constexpr c_fEpsilon = 0.00001f;

	[[nodiscard]] inline bool Approximity(double const a, double const b, double const epsilon = c_dEpsilon) NABI_NOEXCEPT
	{
		return std::fabs(a - b) < epsilon;
	}
	[[nodiscard]] inline bool Approximity(float const a, float const b, float const epsilon = c_fEpsilon) NABI_NOEXCEPT
	{
		return std::fabs(a - b) < epsilon;
	}
} // namespace nabi::Utils::MathUtils
