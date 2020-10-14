#ifndef CAMERA_H
#define CAMERA_H

#include "../Math/D3DMath.h"

// Base camera can be derived into a Camera (read: viewing camera) and a Shadow Camera
class BaseCamera
{
public:

	BaseCamera();
	
	void SetEyeAtUp(Vector3 eye_, Vector3 at_, Vector3 up_);
	void SetPosition(Vector3 position_);
	void SetLookDirection(Vector3 forward_, Vector3 up_ = Vector3(kYUnitVector));
	void SetRotation(Quaternion rotation_);

	void UpdateCamera();

	Vector3 GetPosition() { return m_eye; }

	Vector3 Eye() { return m_eye; }
	Vector3 LookAt() { return m_lookAt; }

	const Matrix4& GetViewMatrix() const { return m_viewMatrix; }
	const Matrix4& GetProjMatrix() const { return m_projectionMatrix; }
	const Matrix4& GetViewProjMatrix() const { return m_viewProjMatrix; }

protected:

	Matrix4 m_viewMatrix;
	Matrix4 m_projectionMatrix;
	Matrix4 m_inverseView;

	Matrix4 m_viewProjMatrix;
	Matrix4 m_previousViewProjMatrix;

	// Frustum m_frustumVS;
	// Frustum m_frustumWS;

	void SetProjectionMatrix(const Matrix4& projMat_) { m_projectionMatrix = projMat_; }

	Vector3 m_eye;
	Vector3 m_lookAt;
	Vector3 m_up;

	Vector3 m_forward;
	Vector3 m_right;
};

class Camera : public BaseCamera
{
public:

	Camera();

	void SetPerspective(float fieldOfView_, float aspectRatio_, float nearPlane_, float farPlane_);

	void SetFOV(float fov_) { m_fieldOfView = fov_; UpdateProjectionMatrix(); }
	void SetAspectRatio(float aspectRatio_) { m_aspectRatio = aspectRatio_; UpdateProjectionMatrix(); }
	void SetZRange(float near_, float far_) { m_nearPlane = near_; m_farPlane = far_; UpdateProjectionMatrix(); }
	float GetFieldOfView() { return m_fieldOfView; }
	float GetNearClipPlane() { return m_nearPlane; }
	float GetFarClipPlane() { return m_farPlane; }
	float GetPitch() { return m_pitch; }
	float GetYaw() { return m_yaw; }

private:

	void UpdateProjectionMatrix();

	float m_pitch, m_yaw, m_roll;
	float m_fieldOfView;
	float m_aspectRatio;
	float m_nearPlane, m_farPlane;
};

inline void BaseCamera::SetEyeAtUp(Vector3 eye_, Vector3 at_, Vector3 up_)
{
	SetPosition(eye_);
	SetLookDirection(at_, up_);
}

inline void BaseCamera::SetPosition(Vector3 position_)
{
	m_eye = position_;
}

inline void BaseCamera::SetRotation(Quaternion rotation_)
{
}

inline Camera::Camera()
{
	SetPerspective(DirectX::XM_PI / 4.0f, 16.0f / 9.0f, 1.0f, 1000.0f);

	SetZRange(0.1f, 10000.0f);
	SetEyeAtUp(Vector3(0.0f, 2.0f, 10.0f), Vector3(kZero), Vector3(kYUnitVector));
}

inline void Camera::SetPerspective(float fieldOfView_, float aspectRatio_, float nearPlane_, float farPlane_)
{
	m_fieldOfView = fieldOfView_;
	m_aspectRatio = aspectRatio_;
	m_nearPlane = nearPlane_;
	m_farPlane = farPlane_;

	UpdateProjectionMatrix();

	m_previousViewProjMatrix = m_viewProjMatrix;
}

#endif