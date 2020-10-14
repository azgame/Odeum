#include "Transform.h"

Transform::Transform()
{
	m_transform = Matrix4();
}

Matrix4 Transform::ScaleFromVector(Vector3 vec)
{
	return Matrix4::MakeScale(vec);
}

Matrix4 Transform::ScaleFromFloat(float scale)
{
	return Matrix4::MakeScale(scale);
}

Matrix4 Transform::ScaleFromFloat(float scaleX, float scaleY, float scaleZ)
{
	return Matrix4::MakeScale(scaleX, scaleY, scaleZ);
}

Matrix4 Transform::RotationFromQuaternion(Quaternion quat)
{
	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	Vector4 q = quat.GetVector4();

	Matrix4 m;
	// first row 
	m.GetX().SetX(1 - (2 * q.GetY() * q.GetY()) - (2 * q.GetZ() * q.GetZ())); // 1 - 2*y2 - 2*z2
	m.GetX().SetY((2 * q.GetX() * q.GetY()) - (2 * q.GetZ() * q.GetW()));
	m.GetX().SetZ((2 * q.GetX() * q.GetZ()) + (2 * q.GetY() * q.GetW()));

	// second row
	m.GetY().SetX((2 * q.GetX() * q.GetY()) + (2 * q.GetZ() * q.GetW()));
	m.GetY().SetY(1 - (2 * q.GetX() * q.GetX()) - (2 * q.GetZ() * q.GetZ()));
	m.GetY().SetZ((2 * q.GetY() * q.GetZ()) - (2 * q.GetX() * q.GetW()));

	// third row
	m.GetZ().SetX((2 * q.GetX() * q.GetZ()) - (2 * q.GetY() * q.GetW()));
	m.GetZ().SetY((2 * q.GetY() * q.GetZ()) + (2 * q.GetX() * q.GetW()));
	m.GetZ().SetZ(1 - (2 * q.GetX() * q.GetX()) - (2 * q.GetY() * q.GetY()));
	return m;
}

Matrix4 Transform::TranslationFromVector(Vector3 vec)
{
	Matrix4 m;
	m.GetX().SetW(vec.GetX());
	m.GetY().SetW(vec.GetY());
	m.GetZ().SetW(vec.GetZ());
	return m;
}