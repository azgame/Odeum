#ifndef VECTOR_H
#define VECTOR_H

// Many of these functions were adapted from the DirectX graphics samples github repo.

#include "../../pch.h"

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




class Vector3 {
public:
	inline Vector3() { vec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	inline Vector3(float x_, float y_, float z_) { vec = DirectX::XMVectorSet(x_, y_, z_, z_); }
	inline Vector3(const DirectX::XMFLOAT3& v_) { vec = XMLoadFloat3(&v_); }
	inline Vector3(const DirectX::XMVECTOR& v_) { vec = v_; }
	inline Vector3(Scalar s) { vec = s.GetVec(); }
	
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
	inline Vector4(Scalar& s_) { vec = s_.GetVec(); }

	inline DirectX::XMVECTOR GetVec() { return vec; }
protected:
	DirectX::XMVECTOR vec;
};

#endif
