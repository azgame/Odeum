#include "Camera.h"

#include <math.h>

void Camera::SetLookDirection(Vector3 forward_, Vector3 up_)
{
	
}

void Camera::UpdateCamera()
{
	// m_previousViewProjMatrix = m_viewProjMatrix;

	// Set view
	// Set viewproj
	// Set frustumVS
	// Set frustumWS
}

void Camera::UpdateProjectionMatrix()
{
	float y = 1.0f / std::tanf(m_fieldOfView * 0.5f);
	float x = y * m_aspectRatio;

	float q1, q2;

	q1 = m_farPlane / (m_nearPlane - m_farPlane);
	q2 = q1 * m_farPlane;

	// SetProjectionMatrix( Matrix4(
	//		Vector4( x, 0.0f, 0.0f, 0.0f ),
	//		Vector4( 0.0f, y, 0.0f, 0.0f ),
	//		Vector4( 0.0f, 0.0f, q1, -1.0f ),
	//		Vector4( 0.0f, 0.0f, q2, 0.0f )
	//	) );
}