#include "Camera.h"

using namespace DirectX;

const float ROTATION_GAIN = 0.004f;
const float MOVEMENT_GAIN = 0.07f;

Camera::Camera()
{
	m_pitch = m_yaw = 0;

	SetViewMatrix(
		XMFLOAT3(0.0f, 0.0f, -5.0f),   // Eye pos
		XMFLOAT3(0.0f, 0.0f, 0.0f),   // Look at
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
	forward = lookAt;
	right = DirectX::XMFLOAT3(m_lookAt.z, m_lookAt.y, -m_lookAt.x);
	m_up = up;

	// Calculate the view matrix.
	XMMATRIX view = XMMatrixLookAtLH(
		XMLoadFloat3(&m_eye),
		XMLoadFloat3(&m_lookAt),
		XMLoadFloat3(&m_up)
	);

	XMVECTOR det;
	XMMATRIX inverseView = XMMatrixInverse(&det, view);
	XMStoreFloat4x4(&m_viewMatrix, view);
	XMStoreFloat4x4(&m_inverseView, inverseView);

	// TODO - Aidan: the fuck does this do?
	/*XMFLOAT3 zBasis;
	XMStoreFloat3(&zBasis, inverseView.r[2]);

	m_yaw = atan2f(zBasis.x, zBasis.z);

	float len = sqrtf(zBasis.z * zBasis.z + zBasis.x * zBasis.x);
	m_pitch = atan2f(zBasis.y, len);*/
}

void Camera::SetProjMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	m_fieldOfView = fieldOfView;
	m_aspectRatio = aspectRatio;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	XMStoreFloat4x4(
		&m_projectionMatrix,
		XMMatrixPerspectiveFovLH(
			m_fieldOfView,
			m_aspectRatio,
			m_nearPlane,
			m_farPlane
		)
	);
}

void Camera::SetLookDirection(DirectX::XMFLOAT3 lookDirection)
{
	forward.x = -lookDirection.x;
	forward.y = -lookDirection.y;
	forward.z = -lookDirection.z;
	right = DirectX::XMFLOAT3(forward.z, forward.y, -forward.x);
}

void Camera::SetEye(DirectX::XMFLOAT3 position)
{
	SetViewMatrix(position, m_lookAt, m_up);
}

void Camera::UpdateCamera()
{
	DirectX::XMFLOAT3 move, view;
	move = m_eye;
	view = m_lookAt;

	float horizontal = 0;
	if (Input::GetInstance()->kb.D)
		horizontal = -0.01f;
	else if (Input::GetInstance()->kb.A)
		horizontal = 0.01f;

	float vertical = 0;
	if (Input::GetInstance()->kb.W)
		vertical = -0.01f;
	else if (Input::GetInstance()->kb.S)
		vertical = 0.01f;

	Input::GetInstance()->m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);

	if (Input::GetInstance()->mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(float(Input::GetInstance()->mouse.x) * ROTATION_GAIN, float(Input::GetInstance()->mouse.y) * ROTATION_GAIN, 0.f * ROTATION_GAIN);
		m_pitch -= delta.y;
		m_yaw -= delta.x;

		float limit = DirectX::XM_PI / 2.0f - 0.01f;

		m_pitch = max(-limit, m_pitch);
		m_pitch = min(limit, m_pitch);

		if (m_yaw > DirectX::XM_PI)
		{
			m_yaw -= DirectX::XM_PI;
		}
		else if (m_yaw < -DirectX::XM_PI)
		{
			m_yaw += DirectX::XM_PI;
		}
	}

	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);

	DirectX::XMFLOAT3 up;
	up.x = 0; up.y = 1; up.z = 0;
	DirectX::XMFLOAT3 look;

	move.x -= vertical * x + horizontal * -z;
	move.y -= vertical * y;
	move.z += vertical * z + horizontal * x;

	look.x = move.x + x; look.y = move.y + y; look.z = move.z - z;
	SetLookDirection(DirectX::XMFLOAT3(x, y, -z));
	SetViewMatrix(move, look, up);
}
