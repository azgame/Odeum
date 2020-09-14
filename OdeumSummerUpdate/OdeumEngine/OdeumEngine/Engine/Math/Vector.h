#ifndef VECTOR_H
#define VECTOR_H

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

#include "../Rendering/DirectX12/DXIncludes.h"
#include <intrin.h>

// made Scalar inside of Vector for ease of access but I might move Scalar to its own class if we end up using it more
class Scalar {
public:
	// creates a 'blank' XMVector
	inline Scalar() { vec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	// creates an XMVector from a float
	inline Scalar(float f_) { vec = DirectX::XMVectorReplicate(f_); }
	// Create a new Scalar from an existing FXMVector
	inline explicit Scalar(DirectX::FXMVECTOR vec_) { vec = vec_; }
	// returns the XMVECTOR, I had to create this function because XMVector functions can't have a scalar given to them.
	inline DirectX::XMVECTOR GetVec() { return vec; }
private:
	DirectX::XMVECTOR vec;
};

// Operator overloads for Scalars, using XMVector functions and overloads.
inline Scalar operator- (Scalar s_) { return Scalar(DirectX::XMVectorNegate(s_.GetVec())); }
inline Scalar operator+ (Scalar s1_, Scalar s2_) { return Scalar(DirectX::XMVectorAdd(s1_.GetVec(), s2_.GetVec())); }
inline Scalar operator- (Scalar s1_, Scalar s2_) { return Scalar(DirectX::XMVectorSubtract(s1_.GetVec(), s2_.GetVec())); }
inline Scalar operator* (Scalar s1_, Scalar s2_) { return Scalar(DirectX::XMVectorMultiply(s1_.GetVec(), s2_.GetVec())); }
inline Scalar operator/ (Scalar s1_, Scalar s2_) { return Scalar(DirectX::XMVectorDivide(s1_.GetVec(), s2_.GetVec())); }
inline Scalar operator+ (Scalar s1_, float s2_) { return s1_ + Scalar(s2_); }
inline Scalar operator- (Scalar s1_, float s2_) { return s1_ - Scalar(s2_); }
inline Scalar operator* (Scalar s1_, float s2_) { return s1_ * Scalar(s2_); }
inline Scalar operator/ (Scalar s1_, float s2_) { return s1_ / Scalar(s2_); }
inline Scalar operator+ (float s1_, Scalar s2_) { return Scalar(s1_) + s2_; }
inline Scalar operator- (float s1_, Scalar s2_) { return Scalar(s1_) - s2_; }
inline Scalar operator* (float s1_, Scalar s2_) { return Scalar(s1_) * s2_; }
inline Scalar operator/ (float s1_, Scalar s2_) { return Scalar(s1_) / s2_; }

// Other miscellaneous functions - might want to move this to a separate header file
inline DirectX::XMVECTOR SplatZero() { return DirectX::XMVectorZero(); }

#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
inline DirectX::XMVECTOR SplatOne(DirectX::XMVECTOR zero_ = SplatZero())
{
	__m128i AllBits = _mm_castps_si128(_mm_cmpeq_ps(zero_, zero_));
	return _mm_castsi128_ps(_mm_slli_epi32(_mm_srli_epi32(AllBits, 25), 23));    // return 0x3F800000
	//return _mm_cvtepi32_ps(_mm_srli_epi32(SetAllBits(zero), 31));                // return (float)1;  (alternate method)
}

#if defined(_XM_SSE_INTRINSICS_)
	inline DirectX::XMVECTOR CreateXUnitVector(DirectX::XMVECTOR one_ = SplatOne())
	{
		return _mm_insert_ps(one_, one_, 0x0E);
	}
	inline DirectX::XMVECTOR CreateYUnitVector(DirectX::XMVECTOR one_ = SplatOne())
	{
		return _mm_insert_ps(one_, one_, 0x0D);
	}
	inline DirectX::XMVECTOR CreateZUnitVector(DirectX::XMVECTOR one_ = SplatOne())
	{
		return _mm_insert_ps(one_, one_, 0x0B);
	}
	inline DirectX::XMVECTOR CreateWUnitVector(DirectX::XMVECTOR one_ = SplatOne())
	{
		return _mm_insert_ps(one_, one_, 0x07);
	}
	inline DirectX::XMVECTOR SetWToZero(DirectX::FXMVECTOR vec_)
	{
		return _mm_insert_ps(vec_, vec_, 0x08);
	}
	inline DirectX::XMVECTOR SetWToOne(DirectX::FXMVECTOR vec_)
	{
		return _mm_blend_ps(vec_, SplatOne(), 0x8);
	}
#else
	inline DirectX::XMVECTOR CreateXUnitVector(DirectX::XMVECTOR one_ = SplatOne())
	{
		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one_), 12));
	}
	inline DirectX::XMVECTOR CreateYUnitVector(DirectX::XMVECTOR one_ = SplatOne())
	{
		DirectX::XMVECTOR unitx = CreateXUnitVector(one_);
		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one_), 4));
	}
	inline DirectX::XMVECTOR CreateZUnitVector(DirectX::XMVECTOR one_ = SplatOne())
	{
		DirectX::XMVECTOR unitx = CreateXUnitVector(one_);
		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one_), 8));
	}
	inline DirectX::XMVECTOR CreateWUnitVector(DirectX::XMVECTOR one_ = SplatOne())
	{
		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one_), 12));
	}
	inline DirectX::XMVECTOR SetWToZero(DirectX::FXMVECTOR vec_)
	{
		__m128i MaskOffW = _mm_srli_si128(_mm_castps_si128(_mm_cmpeq_ps(vec_, vec_)), 4);
		return _mm_and_ps(vec_, _mm_castsi128_ps(MaskOffW));
	}
	inline DirectX::XMVECTOR SetWToOne(DirectX::FXMVECTOR vec_)
	{
		return _mm_movelh_ps(vec_, _mm_unpackhi_ps(vec_, SplatOne()));
	}
#endif

#else
	inline DirectX::XMVECTOR SplatOne() { return DirectX::XMVectorSplatOne(); }
	inline DirectX::XMVECTOR CreateXUnitVector() { return DirectX::g_XMIdentityR0; }
	inline DirectX::XMVECTOR CreateYUnitVector() { return DirectX::g_XMIdentityR1; }
	inline DirectX::XMVECTOR CreateZUnitVector() { return DirectX::g_XMIdentityR2; }
	inline DirectX::XMVECTOR CreateWUnitVector() { return DirectX::g_XMIdentityR3; }
	inline DirectX::XMVECTOR SetWToZero(DirectX::FXMVECTOR vec_) { return DirectX::XMVectorAndInt(vec_, DirectX::g_XMMask3); }
	inline DirectX::XMVECTOR SetWToOne(DirectX::FXMVECTOR vec_) { return DirectX::XMVectorSelect(DirectX::g_XMIdentityR3, vec_, DirectX::g_XMMask3); }
#endif

	enum EZeroTag { kZero, kOrigin };
	enum EZeroTag { kZero, kOrigin };
	enum EIdentityTag { kOne, kIdentity };
	enum EXUnitVector { kXUnitVector };
	enum EYUnitVector { kYUnitVector };
	enum EZUnitVector { kZUnitVector };
	enum EWUnitVector { kWUnitVector };




class Vector3 {
public:
	inline Vector3() { vec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	inline Vector3(float x_, float y_, float z_) { vec = DirectX::XMVectorSet(x_, y_, z_, z_); }
	inline Vector3(const DirectX::XMFLOAT3& v_) { vec = DirectX::XMLoadFloat3(&v_); }
	inline Vector3(const DirectX::XMVECTOR& v_) { vec = v_; }
	inline Vector3(Scalar s) { vec = s.GetVec(); }
	inline explicit Vector3(Vector4 v);
	// vv that line and the other constructor that uses const DirectX::XMFLOAT3& as a parameter take the same argument error
	//inline explicit Vector3(DirectX::FXMVECTOR vec_) { vec = vec_; }
	inline explicit Vector3(EZeroTag) { vec = SplatZero(); }
	inline explicit Vector3(EIdentityTag) { vec = SplatOne(); }
	inline explicit Vector3(EXUnitVector) { vec = CreateXUnitVector(); }
	inline explicit Vector3(EYUnitVector) { vec = CreateYUnitVector(); }
	inline explicit Vector3(EZUnitVector) { vec = CreateZUnitVector(); }
	
	inline operator DirectX::XMVECTOR() const { return vec; }

	// Getters and setters
	inline DirectX::XMVECTOR GetVec() { return vec; }
	inline Scalar GetX() const { return Scalar(DirectX::XMVectorSplatX(vec)); }
	inline Scalar GetY() const { return Scalar(DirectX::XMVectorSplatY(vec)); }
	inline Scalar GetZ() const { return Scalar(DirectX::XMVectorSplatZ(vec)); }
	inline void SetX(Scalar x_) { vec = DirectX::XMVectorPermute<4, 1, 2, 3>(vec, x_.GetVec()); }
	inline void SetY(Scalar y_) { vec = DirectX::XMVectorPermute<0, 5, 2, 3>(vec, y_.GetVec()); }
	inline void SetZ(Scalar z_) { vec = DirectX::XMVectorPermute<0, 1, 6, 3>(vec, z_.GetVec()); }

	// Operator overloads
	inline Vector3 operator- () const { return Vector3(DirectX::XMVectorNegate(vec)); }
	inline Vector3 operator+ (Vector3 v2_) const { return Vector3(DirectX::XMVectorAdd(vec, v2_.GetVec())); }
	inline Vector3 operator- (Vector3 v2_) const { return Vector3(DirectX::XMVectorSubtract(vec, v2_.GetVec())); }
	inline Vector3 operator* (Vector3 v2_) const { return Vector3(DirectX::XMVectorMultiply(vec, v2_.GetVec())); }
	inline Vector3 operator/ (Vector3 v2_) const { return Vector3(DirectX::XMVectorDivide(vec, v2_.GetVec())); }
	inline Vector3 operator* (Scalar  v2_) const { return *this * Vector3(v2_); }
	inline Vector3 operator/ (Scalar  v2_) const { return *this / Vector3(v2_); }
	inline Vector3 operator* (float  v2_) const { return *this * Scalar(v2_); }
	inline Vector3 operator/ (float  v2_) const { return *this / Scalar(v2_); }

	inline Vector3& operator += (Vector3 v_) { *this = *this + v_; return *this; }
	inline Vector3& operator -= (Vector3 v_) { *this = *this - v_; return *this; }
	inline Vector3& operator *= (Vector3 v_) { *this = *this * v_; return *this; }
	inline Vector3& operator /= (Vector3 v_) { *this = *this / v_; return *this; }

	inline friend Vector3 operator* (Scalar  v1_, Vector3 v2_) { return Vector3(v1_) * v2_; }
	inline friend Vector3 operator/ (Scalar  v1_, Vector3 v2_) { return Vector3(v1_) / v2_; }
	inline friend Vector3 operator* (float   v1_, Vector3 v2_) { return Scalar(v1_) * v2_; }
	inline friend Vector3 operator/ (float   v1_, Vector3 v2_) { return Scalar(v1_) / v2_; }

protected:
	DirectX::XMVECTOR vec;
};

class Vector4 {
public:
	inline Vector4() { vec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	inline Vector4(float x_, float y_, float z_, float w_) { vec = DirectX::XMVectorSet(x_, y_, z_, w_); }
	inline Vector4(Vector3 vec_, float w_) { vec = DirectX::XMVectorSetW(vec_.GetVec(), w_); }
	inline Vector4(Vector4& v_) { vec = v_.GetVec(); }
	inline Vector4(const DirectX::XMVECTOR& v_) { vec = v_; }
	inline Vector4(Scalar& s_) { vec = s_.GetVec(); }
	inline explicit Vector4(Vector3 xyz_) { vec = SetWToOne(xyz_); }
	// vv that line and the other constructor that uses const DirectX::XMVECTOR& as a parameter take the same argument error
	//inline explicit Vector4(DirectX::FXMVECTOR vec_) { vec = vec_; }
	inline explicit Vector4(EZeroTag) { vec = SplatZero(); }
	inline explicit Vector4(EIdentityTag) { vec = SplatOne(); }
	inline explicit Vector4(EXUnitVector) { vec = CreateXUnitVector(); }
	inline explicit Vector4(EYUnitVector) { vec = CreateYUnitVector(); }
	inline explicit Vector4(EZUnitVector) { vec = CreateZUnitVector(); }
	inline explicit Vector4(EWUnitVector) { vec = CreateWUnitVector(); }

	inline operator DirectX::XMVECTOR() const { return vec; }
	
	// Getters and Setters
	inline DirectX::XMVECTOR GetVec() { return vec; }
	inline Scalar GetX() const { return Scalar(DirectX::XMVectorSplatX(vec)); }
	inline Scalar GetY() const { return Scalar(DirectX::XMVectorSplatY(vec)); }
	inline Scalar GetZ() const { return Scalar(DirectX::XMVectorSplatZ(vec)); }
	inline Scalar GetW() const { return Scalar(DirectX::XMVectorSplatW(vec)); }
	inline void SetX(Scalar x_) { vec = DirectX::XMVectorPermute<4, 1, 2, 3>(vec, x_.GetVec()); }
	inline void SetY(Scalar y_) { vec = DirectX::XMVectorPermute<0, 5, 2, 3>(vec, y_.GetVec()); }
	inline void SetZ(Scalar z_) { vec = DirectX::XMVectorPermute<0, 1, 6, 3>(vec, z_.GetVec()); }
	inline void SetW(Scalar w_) { vec = DirectX::XMVectorPermute<0, 1, 2, 7>(vec, w_.GetVec()); }

	// Operator Overloads
	inline Vector4 operator- () const { return Vector4(DirectX::XMVectorNegate(vec)); }
	inline Vector4 operator+ (Vector4 v2_) const { return Vector4(DirectX::XMVectorAdd(vec, v2_.GetVec())); }
	inline Vector4 operator- (Vector4 v2_) const { return Vector4(DirectX::XMVectorSubtract(vec, v2_.GetVec())); }
	inline Vector4 operator* (Vector4 v2_) const { return Vector4(DirectX::XMVectorMultiply(vec, v2_.GetVec())); }
	inline Vector4 operator/ (Vector4 v2_) const { return Vector4(DirectX::XMVectorDivide(vec, v2_.GetVec())); }
	inline Vector4 operator* (Scalar v2_) const { return *this * Vector4(v2_); }
	inline Vector4 operator/ (Scalar v2_) const { return *this / Vector4(v2_); }
	inline Vector4 operator* (float v2_) const { return *this * Scalar(v2_); }
	inline Vector4 operator/ (float v2_) const { return *this / Scalar(v2_); }

	inline Vector4& operator += (Vector4 v_) { *this = *this + v_; return *this; }
	inline Vector4& operator -= (Vector4 v_) { *this = *this - v_; return *this; }
	inline Vector4& operator *= (Vector4 v_) { *this = *this * v_; return *this; }
	inline Vector4& operator /= (Vector4 v_) { *this = *this / v_; return *this; }

	inline friend Vector4 operator* (Scalar v1_, Vector4 v2_) { return Vector4(v1_) * v2_; }
	inline friend Vector4 operator/ (Scalar v1_, Vector4 v2_) { return Vector4(v1_) / v2_; }
	inline friend Vector4 operator* (float v1_, Vector4 v2_) { return Scalar(v1_) * v2_; }
	inline friend Vector4 operator/ (float v1_, Vector4 v2_) { return Scalar(v1_) / v2_; }

protected:
	DirectX::XMVECTOR vec;
};

// vv   not sure if we wanted to include this 

class BoolVector
{
public:
	inline BoolVector(DirectX::FXMVECTOR vec_) { vec = vec_; }
	inline operator DirectX::XMVECTOR() const { return vec; }

protected:
	DirectX::XMVECTOR vec;
};

#endif
