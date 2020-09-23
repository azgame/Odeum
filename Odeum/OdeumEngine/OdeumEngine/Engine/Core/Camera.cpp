#include "Camera.h"

#include <math.h>


BaseCamera::BaseCamera()
{
}

void BaseCamera::SetLookDirection(Vector3 forward_, Vector3 up_)
{
	m_lookAt = forward_;
	m_up = up_;
	m_viewMatrix = Matrix4(DirectX::XMMatrixLookAtLH(
		m_eye,
		m_lookAt,
		m_up
	));	
}

void BaseCamera::UpdateCamera()
{
	m_previousViewProjMatrix = m_viewProjMatrix;

	m_viewMatrix = Matrix4(DirectX::XMMatrixLookAtLH(
		m_eye,
		m_lookAt,
		m_up
	));

	m_viewProjMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::UpdateProjectionMatrix()
{
	float y = 1.0f / std::tanf(m_fieldOfView * 0.5f);
	float x = y * m_aspectRatio;

	float q1, q2;

	q1 = m_farPlane / (m_nearPlane - m_farPlane);
	q2 = q1 * m_farPlane;

	m_projectionMatrix = Matrix4(DirectX::XMMatrixPerspectiveFovLH(
		m_fieldOfView,
		m_aspectRatio,
		m_nearPlane,
		m_farPlane
	));

	/*SetProjectionMatrix( Matrix4(
		Vector4( x, 0.0f, 0.0f, 0.0f ),
		Vector4( 0.0f, y, 0.0f, 0.0f ),
		Vector4( 0.0f, 0.0f, q1, -1.0f ),
		Vector4( 0.0f, 0.0f, q2, 0.0f )
	) );*/
}