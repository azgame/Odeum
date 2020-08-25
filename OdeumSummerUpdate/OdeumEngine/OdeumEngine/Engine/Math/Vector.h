#ifndef VECTOR_H
#define VECTOR_H

// Many of these functions were adapted from the DirectX graphics samples github repo

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
	// returns the XMVECTOR, I had to create this function because XMVector functions can't have a scalar given to them
	inline DirectX::XMVECTOR GetVec() { return vec; }
private:
	DirectX::XMVECTOR vec;
};

// Operator overloads for Scalars, using XMVector functions and overloads
inline Scalar operator- (Scalar s) { return Scalar(DirectX::XMVectorNegate(s.GetVec())); }
inline Scalar operator+ (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorAdd(s1.GetVec(), s2.GetVec())); }
inline Scalar operator- (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorSubtract(s1.GetVec(), s2.GetVec())); }
inline Scalar operator* (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorMultiply(s1.GetVec(), s2.GetVec())); }
inline Scalar operator/ (Scalar s1, Scalar s2) { return Scalar(DirectX::XMVectorDivide(s1.GetVec(), s2.GetVec())); }
inline Scalar operator+ (Scalar s1, float s2) { return s1 + Scalar(s2); }
inline Scalar operator- (Scalar s1, float s2) { return s1 - Scalar(s2); }
inline Scalar operator* (Scalar s1, float s2) { return s1 * Scalar(s2); }
inline Scalar operator/ (Scalar s1, float s2) { return s1 / Scalar(s2); }
inline Scalar operator+ (float s1, Scalar s2) { return Scalar(s1) + s2; }
inline Scalar operator- (float s1, Scalar s2) { return Scalar(s1) - s2; }
inline Scalar operator* (float s1, Scalar s2) { return Scalar(s1) * s2; }
inline Scalar operator/ (float s1, Scalar s2) { return Scalar(s1) / s2; }




class Vector3 {
public:
	inline Vector3() { vec = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	inline Vector3(float x_, float y_, float z_) { vec = DirectX::XMVectorSet(x_, y_, z_, z_); }
	inline Vector3(const DirectX::XMFLOAT3& v_) { vec = XMLoadFloat3(&v_); }
	inline Vector3(const DirectX::XMVECTOR& v_) { vec = v_; }
	inline Vector3(Scalar s) { vec = s.GetVec(); }
	inline DirectX::XMVECTOR GetVec() { return vec; }
	
	// Getters and setters
	inline Scalar GetX() const { return Scalar(DirectX::XMVectorSplatX(vec)); }
	inline Scalar GetY() const { return Scalar(DirectX::XMVectorSplatY(vec)); }
	inline Scalar GetZ() const { return Scalar(DirectX::XMVectorSplatZ(vec)); }
	inline void SetX(Scalar x) { vec = DirectX::XMVectorPermute<4, 1, 2, 3>(vec, x.GetVec()); }
	inline void SetY(Scalar y) { vec = DirectX::XMVectorPermute<0, 5, 2, 3>(vec, y.GetVec()); }
	inline void SetZ(Scalar z) { vec = DirectX::XMVectorPermute<0, 1, 6, 3>(vec, z.GetVec()); }

	inline Vector3 operator- () const { return Vector3(DirectX::XMVectorNegate(vec)); }
	inline Vector3 operator+ (Vector3 v2) const { return Vector3(DirectX::XMVectorAdd(vec, v2.GetVec())); }
	inline Vector3 operator- (Vector3 v2) const { return Vector3(DirectX::XMVectorSubtract(vec, v2.GetVec())); }
	inline Vector3 operator* (Vector3 v2) const { return Vector3(DirectX::XMVectorMultiply(vec, v2.GetVec())); }
	inline Vector3 operator/ (Vector3 v2) const { return Vector3(DirectX::XMVectorDivide(vec, v2.GetVec())); }
	inline Vector3 operator* (Scalar  v2) const { return *this * Vector3(v2); }
	inline Vector3 operator/ (Scalar  v2) const { return *this / Vector3(v2); }
	inline Vector3 operator* (float  v2) const { return *this * Scalar(v2); }
	inline Vector3 operator/ (float  v2) const { return *this / Scalar(v2); }

	inline Vector3& operator += (Vector3 v) { *this = *this + v; return *this; }
	inline Vector3& operator -= (Vector3 v) { *this = *this - v; return *this; }
	inline Vector3& operator *= (Vector3 v) { *this = *this * v; return *this; }
	inline Vector3& operator /= (Vector3 v) { *this = *this / v; return *this; }

	inline friend Vector3 operator* (Scalar  v1, Vector3 v2) { return Vector3(v1) * v2; }
	inline friend Vector3 operator/ (Scalar  v1, Vector3 v2) { return Vector3(v1) / v2; }
	inline friend Vector3 operator* (float   v1, Vector3 v2) { return Scalar(v1) * v2; }
	inline friend Vector3 operator/ (float   v1, Vector3 v2) { return Scalar(v1) / v2; }

protected:
	DirectX::XMVECTOR vec;
};

#endif // !VECTOR_H
