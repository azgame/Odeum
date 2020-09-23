#ifndef MATHUTILITY_H
#define MATHUTILITY_H

namespace Math
{
	// Time step must be normalized to between 0-1
	template<typename T> T Lerp(T value, T dest, float step)
	{
		return value * (1.0f - step) + dest * step;
	}
}

#endif