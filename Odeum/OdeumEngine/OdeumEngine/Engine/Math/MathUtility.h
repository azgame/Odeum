#ifndef MATHUTILITY_H
#define MATHUTILITY_H

#define PI 3.141592653589793238462643383279
// yay or nay on the '*'??
#define DEGREES_TO_RADIANS(val) (val * PI) / 180.0f

namespace Math
{
	// Time step must be normalized to between 0-1
	template<typename T> T Lerp(T value, T dest, float step)
	{
		return value * (1.0f - step) + dest * step;
	}
}

#endif