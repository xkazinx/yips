#pragma once

#include <random>

class Random
{
public:
	std::random_device rseed;
	std::mt19937 _rng;
	

	Random() : _rng(rseed())
	{
		//srand((unsigned int)(time(0)));
	}

	template<typename T>
	T Generate(T min, T max)
	{
		std::uniform_int_distribution<T> dist(min, max);
		return dist(_rng);
		//return T(min + (rand() % max));
	}
};

