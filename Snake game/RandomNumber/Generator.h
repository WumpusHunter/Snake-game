/*
	Random number generator
*/

//--------------------------------------------------------------------

#pragma once
#include <random>
#include <ctime>

//--------------------------------------------------------------------

// Returns random engine
std::default_random_engine& get_rand()
{
	static std::default_random_engine ran;

	// Seed random number generator
	static bool called;
	if (!called) {			// We haven't seed so far
		ran.seed(static_cast<unsigned>(time(0)));
		called = true;
	}

	return ran;
};

// Returns random integer number of range [min:max]
inline int randint(int min, int max)
{
	return std::uniform_int_distribution<>{min, max}(get_rand());
}

// Returns random integer number of range [0:max]
inline int randint(int max)
{
	return randint(0, max);
}

//--------------------------------------------------------------------
