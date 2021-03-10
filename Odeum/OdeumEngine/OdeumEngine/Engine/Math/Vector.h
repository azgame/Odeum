#ifndef VECTOR_H
#define VECTOR_H

#define DEGREES_TO_RADIANS PI / 180

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
	inline explicit Vector2(EIdentityTag) { vec = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); }
	inline explicit Vector2(EXUnitVector) { vec = XMVectorSetX(vec, 1.0f); }
	inline explicit Vector2(EYUnitVector) { vec = XMVectorSetY(vec, 1.0f); }

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

	inline bool operator==(Vector2 v_) { return GetX() == v_.GetX() && GetY() == v_.GetY(); }
	inline bool operator!=(Vector2 v_) { return GetX() != v_.GetX() || GetY() != v_.GetY(); }

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
	inline explicit Vector3(Vector4 v);
	//inline explicit Vector3(FXMVECTOR vec_) { vec = vec_; }
	inline explicit Vector3(EZeroTag) { vec = XMVectorZero(); }
	inline explicit Vector3(EIdentityTag) { vec = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); }
	inline explicit Vector3(EXUnitVector) { vec = CreateXUnitVector(); }
	inline explicit Vector3(EYUnitVector) { vec = CreateYUnitVector(); }
	inline explicit Vector3(EZUnitVector) { vec = CreateZUnitVector(); }
	
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

	// Operator overloads
	inline Vector3 operator- () const { return Vector3(XMVectorNegate(vec)); }
	inline Vector3 operator+ (Vector3 v2_) const { return Vector3(XMVectorAdd(vec, v2_.GetVec())); }
	inline Vector3 operator- (Vector3 v2_) const { return Vector3(XMVectorSubtract(vec, v2_.GetVec())); }
	inline Vector3 operator* (Vector3 v2_) const { return Vector3(XMVectorMultiply(vec, v2_.GetVec())); }
	inline Vector3 operator* (float s_) const { return vec * s_; }
	inline Vector3 operator/ (Vector3 v2_) const { return Vector3(XMVectorDivide(vec, v2_.GetVec())); }
	inline Vector3 operator/ (float s_) const { return Vector3(GetX() / s_, GetY() / s_, GetZ() / s_); }

	inline Vector3& operator += (Vector3 v_) { *this = *this + v_; return *this; }
	inline Vector3& operator -= (Vector3 v_) { *this = *this - v_; return *this; }
	inline Vector3& operator *= (Vector3 v_) { *this = *this * v_; return *this; }
	inline Vector3& operator *= (float s_) { *this = *this * s_; return *this; }
	inline Vector3& operator /= (Vector3 v_) { *this = *this / v_; return *this; }

	inline bool operator==(Vector3 v_) { return GetX() == v_.GetX() && GetY() == v_.GetY() && GetZ() == v_.GetZ(); }

protected:
	XMVECTOR vec;
};

class Quaternion;

class Vector4 {
public:
	inline Vector4() { vec = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f); }
	inline Vector4(float x_, float y_, float z_, float w_) { vec = XMVectorSet(x_, y_, z_, w_); }
	inline Vector4(Vector3 vec_, float w_) { vec = XMVectorSetW(vec_.GetVec(), w_); }
	inline Vector4(Vector4& v_) { vec = v_.GetVec(); }
	inline Vector4(const XMVECTOR& v_) { vec = v_; }
	inline Vector4(Quaternion q_);
	inline explicit Vector4(Vector3 xyz_) { vec = SetWToOne(xyz_); }
	inline explicit Vector4(EZeroTag) { vec = XMVectorZero(); }
	inline explicit Vector4(EIdentityTag) { vec = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f); }
	inline explicit Vector4(EXUnitVector) { vec = CreateXUnitVector(); }
	inline explicit Vector4(EYUnitVector) { vec = CreateYUnitVector(); }
	inline explicit Vector4(EZUnitVector) { vec = CreateZUnitVector(); }
	inline explicit Vector4(EWUnitVector) { vec = CreateWUnitVector(); }

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

	inline bool operator==(Vector4 v_) { return GetX() == v_.GetX() && GetY() == v_.GetY() && GetZ() == v_.GetZ() && GetW() == v_.GetW(); }

protected:
	XMVECTOR vec;
};

Vector3::Vector3(Vector4 v) { vec = v.GetVec(); }

namespace Math
{
	inline float Dot(const Vector2 v1, const Vector2 v2)
	{
		return (v1.GetX() * v2.GetX()) + (v1.GetY() * v2.GetY());
	}

	inline float Dot(const Vector3 v1, const Vector3 v2)
	{
		return (v1.GetX() * v2.GetX()) + (v1.GetY() * v2.GetY()) + (v1.GetZ() * v2.GetZ());
	}

	inline float Dot(const Vector4 v1, const Vector4 v2)
	{
		return (v1.GetX() * v2.GetX()) + (v1.GetY() * v2.GetY()) + (v1.GetZ() * v2.GetZ()) + (v1.GetW() * v2.GetW());
	}

	inline Vector2 TripleProduct(const Vector2 v1, const Vector2 v2, const Vector2 v3)
	{
		Vector2 retVal;

		float ac = Dot(v1, v3);
		float bc = Dot(v2, v3);

		retVal.SetX(v2.GetX() * ac - v1.GetX() * bc);
		retVal.SetY(v2.GetY() * ac - v1.GetY() * bc);
		
		return retVal;
	}

	inline float PerpendicularDistance(const Vector2 Point, const Vector2 v1, const Vector2 v2)
	{
		return std::abs((v2.GetX() - v1.GetX()) * (v1.GetY() - Point.GetY()) - (v1.GetX() - Point.GetX()) * (v2.GetY() - v1.GetY())) / (sqrt(pow(v2.GetX() - v1.GetX(), 2) + pow(v2.GetY() - v1.GetY(), 2)));
	}

	inline Vector3 Cross(const Vector3 v1, const Vector3 v2)
	{
		return Vector3(v1.GetY() * v2.GetZ() - v1.GetZ() * v2.GetY(),
			v1.GetZ() * v2.GetX() - v1.GetX() * v2.GetZ(),
			v1.GetX() * v2.GetY() - v1.GetY() * v2.GetX());
	}

	// Courtesy of Scott
	// Reflects off a normal
	inline Vector3 Reflect(const Vector3 v, const Vector3 n)
	{
		/*Vector3 result;
		float scalar = 2.0f * Dot(v, n);
		Vector3 temp = n * scalar;
		result = temp - v;
		*/
		return (n * (2.0f * Dot(v, n))) - v;
	}
	inline Vector4 Reflect(const Vector4 v, const Vector4 n)
	{
		/*Vector3 result;
		float scalar = 2.0f * Dot(v, n);
		Vector3 temp = n * scalar;
		result = temp - v;
		*/
		return (n * (2.0f * Dot(v, n))) - v;
	}
	// Reflects off a plane
	// when uncommenting this function, make sure to comment out the first 4 lines of the function (as seen above)
	/*inline Vector3 Reflect(const Vector3 v, const Plane p)
	{
		Vector3 result;
		float scalar = 2.0f * Dot(v, p);
		Vector3 temp = p * scalar;
		result = temp - v;
		
		return (p * (2.0f * Dot(v, p))) - v;
	}*/

	// Finds the distance between two vectors
	inline float Distance(const Vector3 v1, const Vector3 v2)
	{
		return (v1 - v2).Mag();
	}
	inline float Distance(const Vector4 v1, const Vector4 v2)
	{
		return (v1 - v2).Mag();
	}

	// Finds the distance between a vector and a plane
	/*inline float Distance(const Vector3 v, Plane p)
	{
		return v.GetX() * p.GetX() + v.GetY() * p.GetY() + v.GetZ() * p.GetZ() - p.GetD();
	}*/

	// returns the linear interpolated vector with a give t value
	inline Vector3 Lerp(const Vector3 v1, const Vector3 v2, float t)
	{
		return ((Vector3)v1 + (Vector3)(t * (v2 - v1)));
	}
	inline Vector4 Lerp(const Vector4 v1, const Vector4 v2, float t)
	{
		return ((Vector4)v1 + (Vector4)(t * (v2 - v1)));
	}

	// returns a vector with the minimal values 
	// ** not sure why I want to write everything in a single return statement **
	inline Vector3 Min(const Vector3 v1, const Vector3 v2)
	{
		return Vector3((v1.GetX() < v2.GetX()) ? v1.GetX() : v2.GetX(),
					   (v1.GetY() < v2.GetY()) ? v1.GetY() : v2.GetY(),
					   (v1.GetZ() < v2.GetZ()) ? v1.GetZ() : v2.GetZ());
	}
	inline Vector4 Min(const Vector4 v1, const Vector4 v2)
	{
		return Vector4((v1.GetX() < v2.GetX()) ? v1.GetX() : v2.GetX(),
					   (v1.GetY() < v2.GetY()) ? v1.GetY() : v2.GetY(),
				   	   (v1.GetZ() < v2.GetZ()) ? v1.GetZ() : v2.GetZ(),
					   (v1.GetW() < v2.GetW()) ? v1.GetW() : v2.GetW());
	}

	// returns a vector with the maximum values
	inline Vector3 Max(const Vector3 v1, const Vector3 v2)
	{
		return Vector3((v1.GetX() > v2.GetX()) ? v1.GetX() : v2.GetX(),
					   (v1.GetY() > v2.GetY()) ? v1.GetY() : v2.GetY(),
					   (v1.GetZ() > v2.GetZ()) ? v1.GetZ() : v2.GetZ());
	}
	inline Vector4 Max(const Vector4 v1, const Vector4 v2)
	{
		return Vector4((v1.GetX() > v2.GetX()) ? v1.GetX() : v2.GetX(),
					   (v1.GetY() > v2.GetY()) ? v1.GetY() : v2.GetY(),
					   (v1.GetZ() > v2.GetZ()) ? v1.GetZ() : v2.GetZ(),
					   (v1.GetW() > v2.GetW()) ? v1.GetW() : v2.GetW());
	}


	// STILL NEED TO ADD IN MORE TRANSFORMATION FUNCTIONS
}

#endif
