#ifndef VECTOR_H
#define VECTOR_H

#include "../Rendering/DirectX12/DXIncludes.h"

using namespace DirectX;

//// made Scalar inside of Vector for ease of access but I might move Scalar to its own class if we end up using it more
//class Scalar {
//public:
//	// creates a 'blank' XMVector
//	inline Scalar() { vec = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
//	// creates an XMVector from a float
//	inline Scalar(float f_) { vec = XMVectorReplicate(f_); }
//	// Create a new Scalar from an existing FXMVector
//	inline explicit Scalar(FXMVECTOR vec_) { vec = vec_; }
//	// returns the XMVECTOR, I had to create this function because XMVector functions can't have a scalar given to them.
//	inline XMVECTOR GetVec() { return vec; }
//private:
//	XMVECTOR vec;
//};
//
//// Operator overloads for Scalars, using XMVector functions and overloads.
//inline Scalar operator- (Scalar s_) { return Scalar(XMVectorNegate(s_.GetVec())); }
//inline Scalar operator+ (Scalar s1_, Scalar s2_) { return Scalar(XMVectorAdd(s1_.GetVec(), s2_.GetVec())); }
//inline Scalar operator- (Scalar s1_, Scalar s2_) { return Scalar(XMVectorSubtract(s1_.GetVec(), s2_.GetVec())); }
//inline Scalar operator* (Scalar s1_, Scalar s2_) { return Scalar(XMVectorMultiply(s1_.GetVec(), s2_.GetVec())); }
//inline Scalar operator/ (Scalar s1_, Scalar s2_) { return Scalar(XMVectorDivide(s1_.GetVec(), s2_.GetVec())); }
//inline Scalar operator+ (Scalar s1_, float s2_) { return s1_ + Scalar(s2_); }
//inline Scalar operator- (Scalar s1_, float s2_) { return s1_ - Scalar(s2_); }
//inline Scalar operator* (Scalar s1_, float s2_) { return s1_ * Scalar(s2_); }
//inline Scalar operator/ (Scalar s1_, float s2_) { return s1_ / Scalar(s2_); }
//inline Scalar operator+ (float s1_, Scalar s2_) { return Scalar(s1_) + s2_; }
//inline Scalar operator- (float s1_, Scalar s2_) { return Scalar(s1_) - s2_; }
//inline Scalar operator* (float s1_, Scalar s2_) { return Scalar(s1_) * s2_; }
//inline Scalar operator/ (float s1_, Scalar s2_) { return Scalar(s1_) / s2_; }
//
// Other miscellaneous functions - might want to move this to a separate header file
//inline XMVECTOR SplatZero() { return XMVectorZero(); }
//
//#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)
//inline XMVECTOR SplatOne(XMVECTOR zero_ = SplatZero())
//{
//	__m128i AllBits = _mm_castps_si128(_mm_cmpeq_ps(zero_, zero_));
//	return _mm_castsi128_ps(_mm_slli_epi32(_mm_srli_epi32(AllBits, 25), 23));    // return 0x3F800000
//	//return _mm_cvtepi32_ps(_mm_srli_epi32(SetAllBits(zero), 31));                // return (float)1;  (alternate method)
//}

inline XMVECTOR CreateXUnitVector(XMVECTOR vec = XMVectorZero())
{
	vec = XMVectorSetX(vec, 1.0f);
	return vec;
}

inline XMVECTOR CreateYUnitVector(XMVECTOR vec = XMVectorZero())
{
	vec = XMVectorSetY(vec, 1.0f);
	return vec;
}

inline XMVECTOR CreateZUnitVector(XMVECTOR vec = XMVectorZero())
{
	vec = XMVectorSetZ(vec, 1.0f);
	return vec;
}

inline XMVECTOR CreateWUnitVector(XMVECTOR vec = XMVectorZero())
{
	vec = XMVectorSetW(vec, 1.0f);
	return vec;
}

inline XMVECTOR SetWToZero(XMVECTOR vec)
{
	vec = XMVectorSetW(vec, 0.0f);
	return vec;
}

inline XMVECTOR SetWToOne(XMVECTOR vec)
{
	vec = XMVectorSetW(vec, 1.0f);
	return vec;
}

//#if defined(_XM_SSE_INTRINSICS_)
//	inline XMVECTOR CreateXUnitVector(XMVECTOR one_ = SplatOne())
//	{
//		return _mm_insert_ps(one_, one_, 0x0E);
//	}
//	inline XMVECTOR CreateYUnitVector(XMVECTOR one_ = SplatOne())
//	{
//		return _mm_insert_ps(one_, one_, 0x0D);
//	}
//	inline XMVECTOR CreateZUnitVector(XMVECTOR one_ = SplatOne())
//	{
//		return _mm_insert_ps(one_, one_, 0x0B);
//	}
//	inline XMVECTOR CreateWUnitVector(XMVECTOR one_ = SplatOne())
//	{
//		return _mm_insert_ps(one_, one_, 0x07);
//	}
//	inline XMVECTOR SetWToZero(FXMVECTOR vec_)
//	{
//		return _mm_insert_ps(vec_, vec_, 0x08);
//	}
//	inline XMVECTOR SetWToOne(FXMVECTOR vec_)
//	{
//		return _mm_blend_ps(vec_, SplatOne(), 0x8);
//	}
//#else
//	inline XMVECTOR CreateXUnitVector(XMVECTOR one_ = SplatOne())
//	{
//		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one_), 12));
//	}
//	inline XMVECTOR CreateYUnitVector(XMVECTOR one_ = SplatOne())
//	{
//		XMVECTOR unitx = CreateXUnitVector(one_);
//		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one_), 4));
//	}
//	inline XMVECTOR CreateZUnitVector(XMVECTOR one_ = SplatOne())
//	{
//		XMVECTOR unitx = CreateXUnitVector(one_);
//		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one_), 8));
//	}
//	inline XMVECTOR CreateWUnitVector(XMVECTOR one_ = SplatOne())
//	{
//		return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one_), 12));
//	}
//	inline XMVECTOR SetWToZero(FXMVECTOR vec_)
//	{
//		__m128i MaskOffW = _mm_srli_si128(_mm_castps_si128(_mm_cmpeq_ps(vec_, vec_)), 4);
//		return _mm_and_ps(vec_, _mm_castsi128_ps(MaskOffW));
//	}
//	inline XMVECTOR SetWToOne(FXMVECTOR vec_)
//	{
//		return _mm_movelh_ps(vec_, _mm_unpackhi_ps(vec_, SplatOne()));
//	}
//#endif
//
//#else
//	inline XMVECTOR SplatOne() { return XMVectorSplatOne(); }
//	inline XMVECTOR CreateXUnitVector() { return g_XMIdentityR0; }
//	inline XMVECTOR CreateYUnitVector() { return g_XMIdentityR1; }
//	inline XMVECTOR CreateZUnitVector() { return g_XMIdentityR2; }
//	inline XMVECTOR CreateWUnitVector() { return g_XMIdentityR3; }
//	inline XMVECTOR SetWToZero(FXMVECTOR vec_) { return XMVectorAndInt(vec_, g_XMMask3); }
//	inline XMVECTOR SetWToOne(FXMVECTOR vec_) { return XMVectorSelect(g_XMIdentityR3, vec_, g_XMMask3); }
//#endif

enum EZeroTag { kZero, kOrigin };
enum EIdentityTag { kOne, kIdentity };
enum EXUnitVector { kXUnitVector };
enum EYUnitVector { kYUnitVector };
enum EZUnitVector { kZUnitVector };
enum EWUnitVector { kWUnitVector };

class Vector4;

class Vector3 {
public:
	inline Vector3() { vec = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	inline Vector3(float x_, float y_, float z_) { vec = XMVectorSet(x_, y_, z_, 1.0f); }
	inline Vector3(const XMFLOAT3& v_) { vec = XMLoadFloat3(&v_); }
	inline Vector3(const XMVECTOR& v_) { vec = v_; }
	//inline explicit Vector3(Vector4 v) { vec = v.GetVec(); }
	// vv that line and the other constructor that uses const XMFLOAT3& as a parameter take the same argument error
	//inline explicit Vector3(FXMVECTOR vec_) { vec = vec_; }
	inline explicit Vector3(EZeroTag) { vec = XMVectorZero(); }
	// inline explicit Vector3(EIdentityTag) { vec = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); }
	inline explicit Vector3(EXUnitVector) { vec = CreateXUnitVector(); }
	inline explicit Vector3(EYUnitVector) { vec = CreateYUnitVector(); }
	inline explicit Vector3(EZUnitVector) { vec = CreateZUnitVector(); }
	
	inline operator XMVECTOR() const { return vec; }

	// Getters and setters
	inline XMVECTOR GetVec() { return vec; }
	inline float GetX() const { return XMVectorGetX(vec); }
	inline float GetY() const { return XMVectorGetY(vec); }
	inline float GetZ() const { return XMVectorGetZ(vec); }
	inline void SetX(float x_) { vec = XMVectorSetX(vec, x_); }
	inline void SetY(float y_) { vec = XMVectorSetY(vec, y_); }
	inline void SetZ(float z_) { vec = XMVectorSetZ(vec, z_); }

	// Operator overloads
	inline Vector3 operator- () const { return Vector3(XMVectorNegate(vec)); }
	inline Vector3 operator+ (Vector3 v2_) const { return Vector3(XMVectorAdd(vec, v2_.GetVec())); }
	inline Vector3 operator- (Vector3 v2_) const { return Vector3(XMVectorSubtract(vec, v2_.GetVec())); }
	inline Vector3 operator* (Vector3 v2_) const { return Vector3(XMVectorMultiply(vec, v2_.GetVec())); }
	inline Vector3 operator/ (Vector3 v2_) const { return Vector3(XMVectorDivide(vec, v2_.GetVec())); }
	/*inline Vector3 operator* (Scalar  v2_) const { return *this * Vector3(v2_); }
	inline Vector3 operator/ (Scalar  v2_) const { return *this / Vector3(v2_); }
	inline Vector3 operator* (float  v2_) const { return *this * Scalar(v2_); }
	inline Vector3 operator/ (float  v2_) const { return *this / Scalar(v2_); }*/

	inline Vector3& operator += (Vector3 v_) { *this = *this + v_; return *this; }
	inline Vector3& operator -= (Vector3 v_) { *this = *this - v_; return *this; }
	inline Vector3& operator *= (Vector3 v_) { *this = *this * v_; return *this; }
	inline Vector3& operator /= (Vector3 v_) { *this = *this / v_; return *this; }

	/*inline friend Vector3 operator* (Scalar  v1_, Vector3 v2_) { return Vector3(v1_) * v2_; }
	inline friend Vector3 operator/ (Scalar  v1_, Vector3 v2_) { return Vector3(v1_) / v2_; }
	inline friend Vector3 operator* (float   v1_, Vector3 v2_) { return Scalar(v1_) * v2_; }
	inline friend Vector3 operator/ (float   v1_, Vector3 v2_) { return Scalar(v1_) / v2_; }*/

protected:
	XMVECTOR vec;
};

class Vector4 {
public:
	inline Vector4() { vec = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	inline Vector4(float x_, float y_, float z_, float w_) { vec = XMVectorSet(x_, y_, z_, w_); }
	inline Vector4(Vector3 vec_, float w_) { vec = XMVectorSetW(vec_.GetVec(), w_); }
	inline Vector4(Vector4& v_) { vec = v_.GetVec(); }
	inline Vector4(const XMVECTOR& v_) { vec = v_; }
	// inline Vector4(Scalar& s_) { vec = s_.GetVec(); }
	inline explicit Vector4(Vector3 xyz_) { vec = SetWToOne(xyz_); }
	// vv that line and the other constructor that uses const XMVECTOR& as a parameter take the same argument error
	//inline explicit Vector4(FXMVECTOR vec_) { vec = vec_; }
	inline explicit Vector4(EZeroTag) { vec = XMVectorZero(); }
	// inline explicit Vector4(EIdentityTag) { vec = SplatOne(); }
	inline explicit Vector4(EXUnitVector) { vec = CreateXUnitVector(); }
	inline explicit Vector4(EYUnitVector) { vec = CreateYUnitVector(); }
	inline explicit Vector4(EZUnitVector) { vec = CreateZUnitVector(); }
	inline explicit Vector4(EWUnitVector) { vec = CreateWUnitVector(); }

	inline operator XMVECTOR() const { return vec; }
	
	// Getters and Setters
	inline XMVECTOR GetVec() { return vec; }
	inline float GetX() const { return XMVectorGetX(vec); }
	inline float GetY() const { return XMVectorGetY(vec); }
	inline float GetZ() const { return XMVectorGetZ(vec); }
	inline float GetW() const { return XMVectorGetW(vec); }
	inline void SetX(float x_) { vec = XMVectorSetX(vec, x_); }
	inline void SetY(float y_) { vec = XMVectorSetY(vec, y_); }
	inline void SetZ(float z_) { vec = XMVectorSetZ(vec, z_); }
	inline void SetW(float w_) { vec = XMVectorSetW(vec, w_); }

	// Operator Overloads
	inline Vector4 operator- () const { return Vector4(XMVectorNegate(vec)); }
	inline Vector4 operator+ (Vector4 v2_) const { return Vector4(XMVectorAdd(vec, v2_.GetVec())); }
	inline Vector4 operator- (Vector4 v2_) const { return Vector4(XMVectorSubtract(vec, v2_.GetVec())); }
	inline Vector4 operator* (Vector4 v2_) const { return Vector4(XMVectorMultiply(vec, v2_.GetVec())); }
	inline Vector4 operator/ (Vector4 v2_) const { return Vector4(XMVectorDivide(vec, v2_.GetVec())); }
	//inline Vector4 operator* (Scalar v2_) const { return *this * Vector4(v2_); }
	//inline Vector4 operator/ (Scalar v2_) const { return *this / Vector4(v2_); }
	//inline Vector4 operator* (float v2_) const { return *this * Scalar(v2_); }
	//inline Vector4 operator/ (float v2_) const { return *this / Scalar(v2_); }

	inline Vector4& operator += (Vector4 v_) { *this = *this + v_; return *this; }
	inline Vector4& operator -= (Vector4 v_) { *this = *this - v_; return *this; }
	inline Vector4& operator *= (Vector4 v_) { *this = *this * v_; return *this; }
	inline Vector4& operator /= (Vector4 v_) { *this = *this / v_; return *this; }

	//inline friend Vector4 operator* (Scalar v1_, Vector4 v2_) { return Vector4(v1_) * v2_; }
	//inline friend Vector4 operator/ (Scalar v1_, Vector4 v2_) { return Vector4(v1_) / v2_; }
	//inline friend Vector4 operator* (float v1_, Vector4 v2_) { return Scalar(v1_) * v2_; }
	//inline friend Vector4 operator/ (float v1_, Vector4 v2_) { return Scalar(v1_) / v2_; }

protected:
	XMVECTOR vec;
};

// vv   not sure if we wanted to include this 

class BoolVector
{
public:
	inline BoolVector(FXMVECTOR vec_) { vec = vec_; }
	inline operator XMVECTOR() const { return vec; }

protected:
	XMVECTOR vec;
};

#endif
