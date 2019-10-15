#include "Camera.h"

using namespace DirectX;

Camera::Camera()
{
	SetViewMatrix(
		XMFLOAT3(0.0f, 0.0f, 0.0f),   // Eye pos
		XMFLOAT3(0.0f, 0.0f, 1.0f),   // Look at
		XMFLOAT3(0.0f, 1.0f, 0.0f));  // Up

	SetProjMatrix(XM_PI / 4, 1.0f, 1.0f, 1000.0f);
}


Camera::~Camera()
{
}

void Camera::SetViewMatrix(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 lookAt, DirectX::XMFLOAT3 up)
{
	m_eye = eye;
	m_lookAt = lookAt;
	m_up = up;

	// Calculate the view matrix.
	XMMATRIX view = XMMatrixLookAtRH(
		XMLoadFloat3(&m_eye),
		XMLoadFloat3(&m_lookAt),
		XMLoadFloat3(&m_up)
	);

	XMVECTOR det;
	XMMATRIX inverseView = XMMatrixInverse(&det, view);
	XMStoreFloat4x4(&m_viewMatrix, view);
	XMStoreFloat4x4(&m_inverseView, inverseView);

	// TODO - Aidan: Need to understand this
	XMFLOAT3 zBasis;
	XMStoreFloat3(&zBasis, inverseView.r[2]);

	m_yaw = atan2f(zBasis.x, zBasis.z);

	float len = sqrtf(zBasis.z * zBasis.z + zBasis.x * zBasis.x);
	m_pitch = atan2f(zBasis.y, len);
}

void Camera::SetProjMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	m_fieldOfView = fieldOfView;
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	XMStoreFloat4x4(
		&m_projectionMatrix,
		XMMatrixPerspectiveFovRH(
			m_fieldOfView,
			m_aspectRatio,
			m_nearPlane,
			m_farPlane
		)
	);
}

void Camera::SetLookDirection(DirectX::XMFLOAT3 lookDirection)
{
	XMFLOAT3 lookAt;
	lookAt.x = m_eye.x + lookDirection.x;
	lookAt.y = m_eye.y + lookDirection.y;
	lookAt.z = m_eye.z + lookDirection.z;

	SetViewMatrix(m_eye, lookAt, m_up);
}

void Camera::SetEye(DirectX::XMFLOAT3 position)
{
	SetViewMatrix(position, m_lookAt, m_up);
}