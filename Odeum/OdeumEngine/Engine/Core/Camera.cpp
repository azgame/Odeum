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
		horizontal = -0.05f;
	else if (Input::GetInstance()->kb.A)
		horizontal = 0.05f;

	float vertical = 0;
	if (Input::GetInstance()->kb.W)
		vertical = -0.05f;
	else if (Input::GetInstance()->kb.S)
		vertical = 0.05f;

	float upDown = 0;
	if (Input::GetInstance()->kb.Space)
		upDown = -0.05f;
	else if (Input::GetInstance()->kb.LeftControl)
		upDown = 0.05f;

	Input::GetInstance()->m_mouse->SetMode(DirectX::Mouse::MODE_RELATIVE);

	if (Input::GetInstance()->mouse.positionMode == DirectX::Mouse::MODE_RELATIVE)
	{
		DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(float(Input::GetInstance()->mouse.x) * ROTATION_GAIN, float(Input::GetInstance()->mouse.y) * ROTATION_GAIN, 0.f * ROTATION_GAIN);
		m_pitch -= delta.y;
		m_yaw -= delta.x;

		float limit = DirectX::XM_PI / 2.0f - 0.01f;

		m_pitch = max(-limit, m_pitch);
		m_pitch = min(limit, m_pitch);

		/*if (m_yaw > DirectX::XM_PI)
		{
			m_yaw -= DirectX::XM_PI;
		}
		else if (m_yaw < -DirectX::XM_PI)
		{
			m_yaw += DirectX::XM_PI;
		}*/
	}

	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);

	DirectX::XMFLOAT3 up;
	up.x = 0; up.y = 1; up.z = 0;
	DirectX::XMFLOAT3 look;

	move.x -= vertical * x + horizontal * -z;
	move.y -= vertical * y + upDown;
	move.z += vertical * z + horizontal * x;

	look.x = move.x + x; look.y = move.y + y; look.z = move.z - z;
	SetLookDirection(DirectX::XMFLOAT3(x, y, -z));
	SetViewMatrix(move, look, up);
}

void Camera::GetViewFrustum(std::vector<DirectX::XMFLOAT4>& planes)
{
	DirectX::XMVECTOR look = DirectX::XMLoadFloat3(&m_lookAt);
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_up);
	DirectX::XMVECTOR vright = DirectX::XMLoadFloat3(&right);
	if (m_pitch != 0 || m_yaw != 0 || m_roll != 0) {
		float pitch_rad = DirectX::XMConvertToRadians(m_pitch);
		float yaw_rad = DirectX::XMConvertToRadians(m_yaw);
		float roll_rad = DirectX::XMConvertToRadians(m_roll);

		DirectX::XMMATRIX rot, rotp, roty, rotr;
		rotp = DirectX::XMMatrixRotationAxis(vright, pitch_rad);
		roty = DirectX::XMMatrixRotationAxis(up, yaw_rad);
		rotr = DirectX::XMMatrixRotationAxis(look, roll_rad);
		rot = rotp * roty * rotr;
		look = DirectX::XMVector3Normalize(XMVector3Transform(look, rot));
		vright = DirectX::XMVector3Normalize(XMVector3Transform(vright, rot));
		up = DirectX::XMVector3Cross(vright, look);
	}

	DirectX::XMMATRIX view = DirectX::XMLoadFloat4x4(&m_viewMatrix);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&m_projectionMatrix);
	DirectX::XMFLOAT4X4 M;
	DirectX::XMStoreFloat4x4(&M, view * proj);

	// left
	planes[0].x = M(0, 3) + M(0, 0);
	planes[0].y = M(1, 3) + M(1, 0);
	planes[0].z = M(2, 3) + M(2, 0);
	planes[0].w = M(3, 3) + M(3, 0);

	// right
	planes[1].x = M(0, 3) - M(0, 0);
	planes[1].y = M(1, 3) - M(1, 0);
	planes[1].z = M(2, 3) - M(2, 0);
	planes[1].w = M(3, 3) - M(3, 0);

	// bottom
	planes[2].x = M(0, 3) + M(0, 1);
	planes[2].y = M(1, 3) + M(1, 1);
	planes[2].z = M(2, 3) + M(2, 1);
	planes[2].w = M(3, 3) + M(3, 1);

	// top
	planes[3].x = M(0, 3) - M(0, 1);
	planes[3].y = M(1, 3) - M(1, 1);
	planes[3].z = M(2, 3) - M(2, 1);
	planes[3].w = M(3, 3) - M(3, 1);

	// near
	planes[4].x = M(0, 2);
	planes[4].y = M(1, 2);
	planes[4].z = M(2, 2);
	planes[4].w = M(3, 2);

	// far
	planes[5].x = M(0, 3) - M(0, 2);
	planes[5].y = M(1, 3) - M(1, 2);
	planes[5].z = M(2, 3) - M(2, 2);
	planes[5].w = M(3, 3) - M(3, 2);

	// normalize all planes
	for (auto i = 0; i < 6; ++i) {
		DirectX::XMVECTOR v = DirectX::XMPlaneNormalize(DirectX::XMLoadFloat4(&planes[i]));
		DirectX::XMStoreFloat4(&planes[i], v);
	}
}
