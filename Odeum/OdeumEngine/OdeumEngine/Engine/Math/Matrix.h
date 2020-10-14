#ifndef MATRIX_H
#define MATRIX_H

#include "Quaternion.h"
// Represents a 3x3 matrix while occuping a 4x4 memory footprint.  The unused row and column are undefined but implicitly
// (0, 0, 0, 1).  Constructing a Matrix4 will make those values explicit.
	
//vv   not sure if we want to incorporate _declspec(align(16)) or not
_declspec(align(16))class Matrix3
{
public:
	inline Matrix3() {}
	inline Matrix3(Vector3 x_, Vector3 y_, Vector3 z_) { mat[0] = x_; mat[1] = y_; mat[2] = z_; }
	inline Matrix3(const Matrix3& m_) { mat[0] = m_.mat[0]; mat[1] = m_.mat[1]; mat[2] = m_.mat[2]; }
	inline Matrix3(Quaternion q_) { *this = Matrix3(XMMatrixRotationQuaternion(q_)); }
	inline explicit Matrix3(const XMMATRIX m_) { mat[0] = Vector3(m_.r[0]); mat[1] = Vector3(m_.r[1]); mat[2] = Vector3(m_.r[2]); }
	inline explicit Matrix3(EIdentityTag) { mat[0] = Vector3(kXUnitVector); mat[1] = Vector3(kYUnitVector); mat[2] = Vector3(kZUnitVector); }
	inline explicit Matrix3(EZeroTag) { mat[0] = mat[1] = mat[2] = Vector3(kZero); }
	
	// Setters
	inline void SetX(Vector3 x_) { mat[0] = x_; }
	inline void SetY(Vector3 y_) { mat[1] = y_; }
	inline void SetZ(Vector3 z_) { mat[2] = z_; }

	// Getters
	inline Vector3 GetX() const { return mat[0]; }
	inline Vector3 GetY() const { return mat[1]; }
	inline Vector3 GetZ() const { return mat[2]; }

	static inline Matrix3 MakeXRotation(float angle_) { return Matrix3(XMMatrixRotationX(angle_)); }
	static inline Matrix3 MakeYRotation(float angle_) { return Matrix3(XMMatrixRotationY(angle_)); }
	static inline Matrix3 MakeZRotation(float angle_) { return Matrix3(XMMatrixRotationZ(angle_)); }
	static inline Matrix3 MakeScale(float scale_) { return Matrix3(XMMatrixScaling(scale_, scale_, scale_)); }
	static inline Matrix3 MakeScale(float sx_, float sy_, float sz_) { return Matrix3(XMMatrixScaling(sx_, sy_, sz_)); }
	static inline Matrix3 MakeScale(Vector3 scale_) { return Matrix3(XMMatrixScalingFromVector(scale_)); }

	inline operator XMMATRIX() const { return (const XMMATRIX&)mat; }

	// Operator Overloading
	inline Vector3 operator* (Vector3 vec_) const { return Vector3(XMVector3TransformNormal(vec_, *this)); }
	inline Matrix3 operator* (const Matrix3& mat_) const { return Matrix3(*this * mat_.GetX(), *this * mat_.GetY(), *this * mat_.GetZ()); }

private:
	Vector3 mat[3];
};

// Matrix4
_declspec(align(16)) class Matrix4
{
public:
	inline Matrix4() {}
	inline Matrix4(Vector3 x_, Vector3 y_, Vector3 z_, Vector3 w_)
	{
		mat.r[0] = SetWToZero(x_); mat.r[1] = SetWToZero(y_);
		mat.r[2] = SetWToZero(z_); mat.r[3] = SetWToZero(w_);
	}
	inline Matrix4(Vector4 x_, Vector4 y_, Vector4 z_, Vector4 w_) { mat.r[0] = x_; mat.r[1] = y_; mat.r[2] = z_; mat.r[3] = w_; }
	inline Matrix4(const Matrix4& mat_) { mat = mat_.mat; }
	inline Matrix4(const Matrix3& mat_)
	{
		mat.r[0] = SetWToZero(mat_.GetX());
		mat.r[1] = SetWToZero(mat_.GetY());
		mat.r[2] = SetWToZero(mat_.GetZ());
		mat.r[3] = CreateWUnitVector();
	}
	inline Matrix4(const Matrix3& xyz_, Vector3 w_) {
		mat.r[0] = SetWToZero(xyz_.GetX());
		mat.r[1] = SetWToZero(xyz_.GetY());
		mat.r[2] = SetWToZero(xyz_.GetZ());
		mat.r[3] = SetWToOne(w_);
	}
	// need these from transform
	//inline Matrix4(const AffineTransform& xform_) { *this = Matrix4(xform_.GetBasis(), xform_.GetTranslation()); }
	//inline Matrix4(const OrthogonalTransform& xform_) { *this = Matrix4(Matrix3(xform_.GetRotation()), xform_.GetTranslation()); }
	inline explicit Matrix4(const XMMATRIX& mat_) { mat = mat_; }
	inline explicit Matrix4(EIdentityTag) { mat = XMMatrixIdentity(); }
	inline explicit Matrix4(EZeroTag) { mat.r[0] = mat.r[1] = mat.r[2] = mat.r[3] = XMVectorZero(); }

	inline const Matrix3& Get3x3() const { return (const Matrix3&)*this; }

	// Getters
	inline Vector4 GetX() { return Vector4(mat.r[0]); }
	inline Vector4 GetY() { return Vector4(mat.r[1]); }
	inline Vector4 GetZ() { return Vector4(mat.r[2]); }
	inline Vector4 GetW() { return Vector4(mat.r[3]); }

	// Setters
	inline void SetX(Vector4 x_) { mat.r[0] = x_; }
	inline void SetY(Vector4 y_) { mat.r[1] = y_; }
	inline void SetZ(Vector4 z_) { mat.r[2] = z_; }
	inline void SetW(Vector4 w_) { mat.r[3] = w_; }

	inline operator XMMATRIX() const { return mat; }

	// Operator Overloading
	inline Vector4 operator* (Vector3 vec_) const { return Vector4(XMVector3Transform(vec_, mat)); }
	inline Vector4 operator* (Vector4 vec_) const { return Vector4(XMVector4Transform(vec_, mat)); }
	inline Matrix4 operator* (const Matrix4& mat_) const { return Matrix4(XMMatrixMultiply(mat_, mat)); }

	static inline Matrix4 MakeScale(float scale_) { return Matrix4(XMMatrixScaling(scale_, scale_, scale_)); }
	static inline Matrix4 MakeScale(float scaleX_, float scaleY_, float scaleZ_) { return Matrix4(XMMatrixScaling(scaleX_, scaleY_, scaleZ_)); }
	static inline Matrix4 MakeScale(Vector3 scale_) { return Matrix4(XMMatrixScalingFromVector(scale_)); }

private:
	XMMATRIX mat;
};

#endif