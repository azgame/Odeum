#ifndef EULERANGLES_H
#define EULERANGLES_H

#include "D3DMath.h"

struct EulerAngles
{
	inline EulerAngles() { yaw = pitch = roll = 0.0f; };
	inline EulerAngles(float yaw_, float pitch_, float roll_) { yaw = yaw_; pitch = pitch_; roll = roll_; };

	// member variables
	float yaw, pitch, roll;
};

#endif