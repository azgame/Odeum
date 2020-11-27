#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector.h"
#include "EulerAngles.h"

class Quaternion
{
public:
	// creates the identity quaternion
	inline Quaternion() { vec = XMQuaternionIdentity(); }
	inline explicit Quaternion(EIdentityTag) { vec = XMQuaternionIdentity(); }
	// creates a rotation matrix based on an axis and angle
	// DirectX Lib uses const Scalar& instead of const float&
	inline Quaternion(const XMVECTOR& axis_, const float& angle_) { vec = XMQuaternionRotationAxis(axis_, angle_); }
	// creates a rotation matrix based on the pitch, yaw, and roll
	inline Quaternion(float pitch_, float yaw_, float roll_) { vec = XMQuaternionRotationRollPitchYaw(pitch_, yaw_, roll_); }
	inline explicit Quaternion(const XMMATRIX& matrix_) { vec = XMQuaternionRotationMatrix(matrix_); }
	inline explicit Quaternion(FXMVECTOR vec_) { vec = vec_; }

	inline operator XMVECTOR() const { return vec; }

	inline Vector4 GetVector4() const { return vec; }

	// Operator Overloads
	/* Conjugate*/	inline Quaternion operator~ (void) const { return Quaternion(XMQuaternionConjugate(vec)); }
	/* Inverse	*/	inline Quaternion operator- (void) const { return Quaternion(XMQuaternionInverse(vec)); }

	inline Quaternion operator* (Quaternion rhs_) const { return Quaternion(XMQuaternionMultiply(rhs_, vec)); }
	inline Vector3 operator* (Vector3 rhs_) const { return Vector3(XMVector3Rotate(rhs_, vec)); }

	inline Quaternion& operator= (Quaternion rhs_) { vec = rhs_; return *this; }
	inline Quaternion& operator*= (Quaternion rhs_) { *this = *this * rhs_; return *this; }

protected:
	XMVECTOR vec;
};

Vector4::Vector4(Quaternion q_) { vec = q_.GetVector4(); }

// This is where we got the quaternion to euler calculations
// https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/index.htm

namespace Math {
	inline EulerAngles QuatToEuler(Quaternion q_) {
		// can't seem to access members of quaternion so we must turn it into a vector4 first
		Vector4 q = q_.GetVector4();
		EulerAngles e;

		// we use squared values a lot so make variables for them (doubles more precise)
		double sqw = q.GetW() * q.GetW();
		double sqx = q.GetX() * q.GetX();
		double sqy = q.GetY() * q.GetY();
		double sqz = q.GetZ() * q.GetZ();

		double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
		double test = q.GetX() * q.GetY() + q.GetZ() * q.GetW();

		// 
		if (test > 0.49999 * unit) { // singularity at north pole
			e.yaw = 2 * atan2(q.GetX(), q.GetW());
			e.pitch = (float)(PI / 2.0);
			e.roll = 0;
			return e;
		}
		if (test < -0.49999 * unit) { // singularity at south pole
			e.yaw = -2 * atan2(q.GetX(), q.GetW());
			e.pitch = (float)(-PI / 2.0);
			e.roll = 0;
			return e;
		}
		e.yaw = (float)atan2(2 * q.GetY() * q.GetW() - 2 * q.GetX() * q.GetZ(), sqx - sqy - sqz + sqw);
		e.pitch = (float)asin(2 * test / unit);
		e.roll = (float)atan2(2 * q.GetX() * q.GetW() - 2 * q.GetY() * q.GetZ(), -sqx + sqy - sqz + sqw);

		return e;
	};
}

#endif