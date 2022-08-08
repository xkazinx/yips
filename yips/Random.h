#pragma once
class Random
{
public:
	Random() 
	{
		srand((unsigned int)(time(0)));
	}

	template<typename T>
	T Generate(T min, T max)
	{
		return T(min + (rand() % max));
	}
};

