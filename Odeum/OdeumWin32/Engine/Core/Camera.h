#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../../pch.h"
#include "Input.h"

class Camera
{
public:

	Camera();
	~Camera();

	void SetViewMatrix(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 lookAt, DirectX::XMFLOAT3 up);
	void SetProjMatrix(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);

	void SetLookDirection(DirectX::XMFLOAT3 lookDirection);
	void SetEye(DirectX::XMFLOAT3 position);

	void UpdateCamera();

	float NearClipPlane()				{ return m_nearPlane; }
	float FarClipPlane()				{ return m_farPlane; }
	float Pitch()						{ return m_pitch; }
	float Yaw()							{ return m_yaw; }
	DirectX::XMFLOAT3 Eye()				{ return m_eye; }
	DirectX::XMFLOAT3 LookAt()			{ return m_lookAt; }
	DirectX::XMMATRIX View()			{ return XMLoadFloat4x4(&m_viewMatrix); }
	DirectX::XMMATRIX Projection()		{ return XMLoadFloat4x4(&m_projectionMatrix); }
	DirectX::XMMATRIX World()			{ return XMLoadFloat4x4(&m_inverseView); }

	DirectX::XMFLOAT3					forward;
	DirectX::XMFLOAT3					right;

private:
	DirectX::XMFLOAT4X4					m_viewMatrix;
	DirectX::XMFLOAT4X4					m_projectionMatrix;

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