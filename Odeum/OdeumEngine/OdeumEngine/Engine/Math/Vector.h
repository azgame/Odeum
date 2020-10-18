#ifndef VECTOR_H
#define VECTOR_H

#include "../Rendering/DirectX12/D3DIncludes.h"

using namespace DirectX;

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

enum EZeroTag { kZero, kOrigin };
enum EIdentityTag { kOne, kIdentity };
enum EXUnitVector { kXUnitVector };
enum EYUnitVector { kYUnitVector };
enum EZUnitVector { kZUnitVector };
enum EWUnitVector { kWUnitVector };

class Vector3;

class Vector2
{
public:
	inline Vector2() { vec = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	inline Vector2(float x_, float y_) { vec = XMVectorSet(x_, y_, 1.0f, 1.0f); }
	inline Vector2(const XMFLOAT2& v_) { vec = XMLoadFloat2(&v_); }
	inline Vector2(const XMVECTOR& v_) { vec = v_; }
	//inline explicit Vector3(Vector4 v) { vec = v.GetVec(); }
	// vv that line and the other constructor that uses const XMFLOAT3& as a parameter take the same argument error
	//inline explicit Vector3(FXMVECTOR vec_) { vec = vec_; }
	inline explicit Vector2(EZeroTag) { vec = XMVectorZero(); }
	// inline explicit Vector3(EIdentityTag) { vec = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); }
	inline explicit Vector2(EXUnitVector) { vec = CreateXUnitVector(); }
	inline explicit Vector2(EYUnitVector) { vec = CreateYUnitVector(); }
	inline explicit Vector2(EIdentityTag) { vec = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); }

	inline operator XMVECTOR() const { return vec; }

	// Print
	inline void Print() { std::cout << "(" << GetX() << ", " << GetY() << ")"; }
	inline std::string ToString() const { return std::string("(" + std::to_string(GetX()) + ", " + std::to_string(GetY()) + ")"); }

	// Getters and setters
	inline XMVECTOR GetVec() { return vec; }
	inline float GetX() const { return XMVectorGetX(vec); }
	inline float GetY() const { return XMVectorGetY(vec); }
	inline void SetX(float x_) { vec = XMVectorSetX(vec, x_); }
	inline void SetY(float y_) { vec = XMVectorSetY(vec, y_); }
	//Functions Added -Robert
	inline float Mag() { return sqrt(pow(XMVectorGetX(vec), 2) + pow(XMVectorGetY(vec), 2)); }
	inline Vector2 Normalize() { return vec / sqrt(pow(XMVectorGetX(vec), 2) + pow(XMVectorGetY(vec), 2)); }
	// Operator overloads
	inline Vector2 operator- () const { return Vector2(XMVectorNegate(vec)); }
	inline Vector2 operator+ (Vector2 v2_) const { return Vector2(XMVectorAdd(vec, v2_.GetVec())); }
	inline Vector2 operator- (Vector2 v2_) const { return Vector2(XMVectorSubtract(vec, v2_.GetVec())); }
	inline Vector2 operator* (Vector2 v2_) const { return Vector2(XMVectorMultiply(vec, v2_.GetVec())); }
	inline Vector2 operator/ (Vector2 v2_) const { return Vector2(XMVectorDivide(vec, v2_.GetVec())); }

	inline Vector2& operator += (Vector2 v_) { *this = *this + v_; return *this; }
	inline Vector2& operator -= (Vector2 v_) { *this = *this - v_; return *this; }
	inline Vector2& operator *= (Vector2 v_) { *this = *this * v_; return *this; }
	inline Vector2& operator /= (Vector2 v_) { *this = *this / v_; return *this; }
protected:
	XMVECTOR vec;
};

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
	inline explicit Vector3(EIdentityTag) { vec = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); }
	
	inline operator XMVECTOR() const { return vec; }

	// Print
	inline void Print() { std::cout << "(" << GetX() << ", " << GetY() << ", " << GetZ() << ")"; }
	inline std::string ToString() const { return std::string("(" + std::to_string(GetX()) + ", " + std::to_string(GetY()) + ", " + std::to_string(GetZ()) + ")"); }

	// Getters and setters
	inline XMVECTOR GetVec() { return vec; }
	inline float GetX() const { return XMVectorGetX(vec); }
	inline float GetY() const { return XMVectorGetY(vec); }
	inline float GetZ() const { return XMVectorGetZ(vec); }
	inline void SetX(float x_) { vec = XMVectorSetX(vec, x_); }
	inline void SetY(float y_) { vec = XMVectorSetY(vec, y_); }
	inline void SetZ(float z_) { vec = XMVectorSetZ(vec, z_); }
	//Functions Added -Robert
	inline float Mag() { return sqrt(pow(XMVectorGetX(vec), 2) + pow(XMVectorGetY(vec), 2) + pow(XMVectorGetZ(vec), 2)); }
	inline Vector3 Normalize(){return vec/ sqrt(pow(XMVectorGetX(vec), 2) + pow(XMVectorGetY(vec), 2) + pow(XMVectorGetZ(vec), 2));}
	inline Vector3 Cross(Vector3 otherVec)
	{
		Vector3 t;
		t.SetX( (XMVectorGetX(vec) * otherVec.GetZ()) - (XMVectorGetZ(vec) * otherVec.GetY()));
		t.SetY( (XMVectorGetZ(vec) * otherVec.GetX()) - (XMVectorGetX(vec) * otherVec.GetZ()));
		t.SetZ( (XMVectorGetX(vec) * otherVec.GetY()) - (XMVectorGetY(vec) * otherVec.GetZ()));
		return t;
	}
	



	inline float Dot(Vector3 v) { return (GetX() * v.GetX()) + (GetY() * v.GetY()) + (GetZ() * v.GetZ()); }
	
	inline Vector3 Cross(Vector3 v) 
	{
		return Vector3(GetY() * v.GetZ() - GetZ() * v.GetY(),
			GetZ() * v.GetX() - GetX() * v.GetZ(),
			GetX() * v.GetY() - GetY() * v.GetX());
	}

	// Operator overloads
	inline Vector3 operator- () const { return Vector3(XMVectorNegate(vec)); }
	inline Vector3 operator+ (Vector3 v2_) const { return Vector3(XMVectorAdd(vec, v2_.GetVec())); }
	inline Vector3 operator- (Vector3 v2_) const { return Vector3(XMVectorSubtract(vec, v2_.GetVec())); }
	inline Vector3 operator* (Vector3 v2_) const { return Vector3(XMVectorMultiply(vec, v2_.GetVec())); }
	inline Vector3 operator/ (Vector3 v2_) const { return Vector3(XMVectorDivide(vec, v2_.GetVec())); }

	inline Vector3& operator += (Vector3 v_) { *this = *this + v_; return *this; }
	inline Vector3& operator -= (Vector3 v_) { *this = *this - v_; return *this; }
	inline Vector3& operator *= (Vector3 v_) { *this = *this * v_; return *this; }
	inline Vector3& operator /= (Vector3 v_) { *this = *this / v_; return *this; }

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
	//inline Vector4(Quaternion q_) { vec = q_.GetVec(); }
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
	inline explicit Vector4(EIdentityTag) { vec = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); }

	inline operator XMVECTOR() const { return vec; }

	// Print
	inline void Print() { std::cout << "(" << GetX() << ", " << GetY() << ", " << GetZ() << ", " << GetW() << ")"; }
	inline std::string ToString() const { return std::string("(" + std::to_string(GetX()) + ", " + std::to_string(GetY()) + ", " + std::to_string(GetZ()) + ", " + std::to_string(GetW()) + ")"); }
	
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
	inline float Mag() { return sqrt(pow(XMVectorGetX(vec), 2) + pow(XMVectorGetY(vec), 2) + pow(XMVectorGetZ(vec), 2)+pow(XMVectorGetW(vec),2)); }
	inline Vector4 Normalize() { return vec / sqrt(pow(XMVectorGetX(vec), 2) + pow(XMVectorGetY(vec), 2) + pow(XMVectorGetZ(vec), 2) + pow(XMVectorGetW(vec), 2)); }

	// Operator Overloads
	inline Vector4 operator- () const { return Vector4(XMVectorNegate(vec)); }
	inline Vector4 operator+ (Vector4 v2_) const { return Vector4(XMVectorAdd(vec, v2_.GetVec())); }
	inline Vector4 operator- (Vector4 v2_) const { return Vector4(XMVectorSubtract(vec, v2_.GetVec())); }
	inline Vector4 operator* (Vector4 v2_) const { return Vector4(XMVectorMultiply(vec, v2_.GetVec())); }
	inline Vector4 operator* (float s_) const { return vec * s_; }
	inline Vector4 operator/ (Vector4 v2_) const { return Vector4(XMVectorDivide(vec, v2_.GetVec())); }
	inline Vector4 operator/ (float s_) const { return Vector4(GetX() / s_, GetY() / s_, GetZ() / s_, GetW() / s_); }

	inline Vector4& operator += (Vector4 v_) { *this = *this + v_; return *this; }
	inline Vector4& operator -= (Vector4 v_) { *this = *this - v_; return *this; }
	inline Vector4& operator *= (Vector4 v_) { *this = *this * v_; return *this; }
	inline Vector4& operator *= (float s_) { *this = *this * s_; return *this; }
	inline Vector4& operator /= (Vector4 v_) { *this = *this / v_; return *this; }
	inline Vector4& operator /= (float s_) { *this = *this / s_; return *this; }

protected:
	XMVECTOR vec;
};

namespace Math
{
	inline float Dot(Vector3 v1, Vector3 v2)
	{
		return (v1.GetX() * v2.GetX()) + (v1.GetY() * v2.GetY()) + (v1.GetZ() * v2.GetZ());
	}
}

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
