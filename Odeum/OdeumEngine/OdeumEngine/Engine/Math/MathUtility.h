#ifndef MATHUTILITY_H
#define MATHUTILITY_H

#define PI 3.141592653589793238462643383279
// yay or nay on the '*'??
#define DEGREES_TO_RADIANS(val) (val * PI) / 180.0f

#include <random>

namespace Math
{
	// Time step must be normalized to between 0-1
	template<typename T> T Lerp(T value, T dest, float step)
	{
		return value * (1.0f - step) + dest * step;
	}

	template<typename T> T RandValInRange(T low, T high)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<T> distribution(low, high);

		return distribution(gen);
	}

	int RandIntInRange(int low, int high)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribution(low, high);

		return distribution(gen);
	}
}

#endif