#pragma once

#include "DebugUtils.h"

namespace nabi::Utils::MathUtils
{
	// --- Random Number Generations ---

	/*
	  It might be good to move away from rand() in the future? But for now its fine.
	  Perhaps https://prng.di.unimi.it/xoshiro256plusplus.c?
	*/

	/// <summary>
	/// Generates a random number in the range of [0] - [max]
	/// </summary>
	/// <param name="max">- The maximum random number that will be generated</param>
	/// <returns>A random number</returns>
	static int GenerateRandomNumber(int const max)
	{
		return (rand() % max) + 1;
	}

	/// <summary>
	/// Creates a random number in the range of [min] - [max]
	/// </summary>
	/// <param name="min">- The minimum random number that will be generated</param>
	/// <param name="max">- The maximum random number that will be generated</param>
	/// <returns>A random number</returns>
	static int GenerateRandomNumberInRange(int const min, int const max)
	{
		ASSERT(min < max, "Trying to generate a random number in a range but [min] (" << min << ") is greater than or equal to [max] (" << max << ")!");
		return min + rand() % (max - min + 1);
	}
} // namespace nabi::Utils::MathUtils
