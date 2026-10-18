#pragma once

#include <random>

class Random
{
public:
	std::random_device _seed;
	std::mt19937 _rng;

	Random() : _rng(_seed()) {}

	template<typename T>
	T Generate(const T min, const T max)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(_rng);
		}
		else
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(_rng);
		}
	}

	// One of the enum's values, never None
	template<typename T>
	T GenerateEnumerator()
	{
		constexpr auto kValues = enum_values<T>();
		constexpr szt kCount = [] {
			szt n = 0;

			for (const auto v : enum_values<T>())
				n += v != T::None;

			return n;
		}();
		static_assert(kCount > 0);
		return kValues[Generate<szt>(0, kCount - 1)];
	}
};
