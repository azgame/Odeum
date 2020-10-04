#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"

class Quaternion
{
public:
	// creates the identity quaternion
	inline Quaternion() { vec = XMQuaternionIdentity(); }
	// creates a rotation matrix based on an axis and angle
	// DirectX Lib uses const Scalar& instead of const float&
	inline Quaternion(const XMVECTOR& axis_, const float& angle_) { vec = XMQuaternionRotationAxis(axis_, angle_); }
	// creates a rotation matrix based on the pitch, yaw, and roll
	inline Quaternion(float pitch_, float yaw_, float roll_) { vec = XMQuaternionRotationRollPitchYaw(pitch_, yaw_, roll_); }
	inline explicit Quaternion(const XMMATRIX& matrix_) { vec = XMQuaternionRotationMatrix(matrix_); }
	inline explicit Quaternion(FXMVECTOR vec_) { vec = vec_; }
	inline explicit Quaternion(EIdentityTag) { vec = XMQuaternionIdentity(); }

	inline operator XMVECTOR() const { return vec; }

	inline Vector4 GetVector4() const { return vec; }

	// Operator Overloads
	// creates the conjugate of the quaternion
	inline Quaternion operator~ (void) const { return Quaternion(XMQuaternionConjugate(vec)); }

	// creates inverse I believe?
	inline Quaternion operator- (void) const { return Quaternion(XMVectorNegate(vec)); }

	inline Quaternion operator* (Quaternion rhs_) const { return Quaternion(XMQuaternionMultiply(rhs_, vec)); }
	inline Vector3 operator* (Vector3 rhs_) const { return Vector3(XMVector3Rotate(rhs_, vec)); }

	inline Quaternion& operator= (Quaternion rhs_) { vec = rhs_; return *this; }
	inline Quaternion& operator*= (Quaternion rhs_) { *this = *this * rhs_; return *this; }

protected:
	XMVECTOR vec;
};




#endif