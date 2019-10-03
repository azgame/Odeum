#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "pch.h"

class Camera
{
public:

	Camera();
	~Camera();

	void SetViewMatrix(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 lookAt, DirectX::XMFLOAT3 up);
	void SetProjMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	void LookDirection(DirectX::XMFLOAT3 lookDirection);
	void Eye(DirectX::XMFLOAT3 position);

	float NearClipPlane()				{ return m_nearPlane; }
	float FarClipPlane()				{ return m_farPlane; }
	float Pitch()						{ return m_pitch; }
	float Yaw()							{ return m_yaw; }

private:
	DirectX::XMFLOAT4X4					m_viewMatrix;
	DirectX::XMFLOAT4X4					m_projectionMatrix;
	DirectX::XMFLOAT4X4					m_projectionMatrixLeft;
	DirectX::XMFLOAT4X4					m_projectionMatrixRight;

	DirectX::XMFLOAT4X4					m_inverseView;

	DirectX::XMFLOAT3					m_eye;
	DirectX::XMFLOAT3					m_lookAt;
	DirectX::XMFLOAT3					m_up;

	float								m_pitch, m_yaw, m_roll;
	float								m_fieldOfView;
	float								m_aspectRatio;
	float								m_nearPlane;
	float								m_farPlane;
};

#endif // !_CAMERA_H_