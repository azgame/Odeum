#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Matrix.h"

class Transform
{
public:
	Transform();

	Matrix4 ScaleFromVector(Vector3 vec);
	Matrix4 ScaleFromFloat(float scale);
	Matrix4 ScaleFromFloat(float scaleX, float scaleY, float scaleZ);
	static Matrix4 RotationFromQuaternion(Quaternion quat);
	Matrix4 TranslationFromVector(Vector3 vec);

	Matrix4 Update();

private:
	Matrix4 m_transform;
};

#endif

