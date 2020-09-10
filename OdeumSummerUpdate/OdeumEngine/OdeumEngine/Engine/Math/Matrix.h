#ifndef MATRIX_H
#define MATRIX_H

// Many of these functions were adapted from the DirectX graphics samples github repo.

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

#include "Quaternion.h"
// Represents a 3x3 matrix while occuping a 4x4 memory footprint.  The unused row and column are undefined but implicitly
// (0, 0, 0, 1).  Constructing a Matrix4 will make those values explicit.
	
//vv   not sure if we want to incorporate _declspec(align(16)) or not
_declspec(align(16))class Matrix3
{
public:
	inline Matrix3() {}
	inline Matrix3(Vector3 x_, Vector3 y_, Vector3 z_) { mat[0] = x_; mat[1] = y_; mat[2] = z_; }
	inline Matrix3(const Matrix3& m_) { m[0] = m_.m[0]; m[1] = m_.m[1]; m[2] = m_.m[2]; }
	inline Matrix3(Quaternion q_) { *this = Matrix3(DirectX::XMMatrixRotationQuaternion(q_));  }
private:
	Vector3 mat[3];
};

class Matrix4
{
};


#endif
