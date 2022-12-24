#pragma once

#include <stdint.h> // uint64_t == unsigned long long

namespace nabi::Utils::MathUtils
{
	// --- Random Number Generations ---

	/// <summary>
	/// Generates a random number in the range of [0] - [max]
	/// </summary>
	/// <param name="max">- The maximum random number that will be generated</param>
	/// <returns>A random number</returns>
	static uint64_t GenerateRandomNumber(uint64_t const max)
	{
		return (next() % max) + 1;
	}

	/// <summary>
	/// Creates a random number in the range of [min] - [max]
	/// </summary>
	/// <param name="min">- The minimum random number that will be generated</param>
	/// <param name="max">- The maximum random number that will be generated</param>
	/// <returns>A random number</returns>
	static uint64_t GenerateRandomNumberInRange(uint64_t const min, uint64_t const max)
	{
		return min + next() % (max - min + 1);
	}
	
	// Helper functions for random number generation
	namespace
	{
		/*
		  Taken from https://prng.di.unimi.it/xoshiro256plusplus.c
		  Under CC0 1.0 (see http://creativecommons.org/publicdomain/zero/1.0/)
		  Credit to David Blackman and Sebastiano Vigna (vigna@acm.org) (2019)
		*/

		static uint64_t s[4];

		static inline uint64_t rotl(const uint64_t x, int k) {
			return (x << k) | (x >> (64 - k));
		}

		static uint64_t next() {
			const uint64_t result = rotl(s[0] + s[3], 23) + s[0];

			const uint64_t t = s[1] << 17;

			s[2] ^= s[0];
			s[3] ^= s[1];
			s[1] ^= s[2];
			s[0] ^= s[3];

			s[2] ^= t;

			s[3] = rotl(s[3], 45);

			return result;
		}
	} 
} // namespace nabi::Utils::MathUtils
