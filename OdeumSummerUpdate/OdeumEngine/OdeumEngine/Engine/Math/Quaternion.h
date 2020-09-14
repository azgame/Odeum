#ifndef QUATERNION_H
#define QUATERNION_H

//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
// Developed by Minigraph
//
// Author:  James Stanard 
//


#include "Vector.h"

class Quaternion
{
public:
	// creates the identity quaternion
	inline Quaternion() { vec = DirectX::XMQuaternionIdentity(); }
	// creates a rotation matrix based on an axis and angle
	// DirectX Lib uses const Scalar& instead of const float&
	inline Quaternion(const DirectX::XMVECTOR& axis_, const float& angle_) { vec = DirectX::XMQuaternionRotationAxis(axis_, angle_); }
	// creates a rotation matrix based on the pitch, yaw, and roll
	inline Quaternion(float pitch_, float yaw_, float roll_) { vec = DirectX::XMQuaternionRotationRollPitchYaw(pitch_, yaw_, roll_); }
	inline explicit Quaternion(const DirectX::XMMATRIX& matrix_) { vec = DirectX::XMQuaternionRotationMatrix(matrix_); }
	inline explicit Quaternion(DirectX::FXMVECTOR vec_) { vec = vec_; }
	inline explicit Quaternion(EIdentityTag) { vec = DirectX::XMQuaternionIdentity(); }

	inline operator DirectX::XMVECTOR() const { return vec; }

	// Operator Overloads
	// creates the conjugate of the quaternion
	inline Quaternion operator~ (void) const { return Quaternion(DirectX::XMQuaternionConjugate(vec)); }

	// creates inverse I believe?
	inline Quaternion operator- (void) const { return Quaternion(DirectX::XMVectorNegate(vec)); }

	inline Quaternion operator* (Quaternion rhs_) const { return Quaternion(DirectX::XMQuaternionMultiply(rhs_, vec)); }
	inline Vector3 operator* (Vector3 rhs_) const { return Vector3(DirectX::XMVector3Rotate(rhs_, vec)); }

	inline Quaternion& operator= (Quaternion rhs_) { vec = rhs_; return *this; }
	inline Quaternion& operator*= (Quaternion rhs_) { *this = *this * rhs_; return *this; }

protected:
	DirectX::XMVECTOR vec;
};




#endif