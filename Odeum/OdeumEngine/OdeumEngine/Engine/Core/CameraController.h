#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

class CameraController
{
public:
	void UpdateMainCamera();

	const float ROTATION_GAIN = 0.004f;
	const float MOVEMENT_GAIN = 0.07f;
};

#endif