#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "../Math/D3DMath.h"

class CameraController
{
public:
	void UpdateMainCamera();

	const float ROTATION_GAIN = 0.004f;
	const float MOVEMENT_GAIN = 0.07f;

private:

	float m_pitch = 0.0f, m_yaw = 0.0f, m_roll = 0.0f;
	Vector2 lastMouse;
};

#endif