#include "CameraController.h"
#include "OdeumEngine.h"
#include "Camera.h"

void CameraController::UpdateMainCamera()
{
	Camera& mainCamera = OdeumEngine::Get().GetCamera();
	Vector3 move = mainCamera.GetPosition();

	float xPlane = 0;
	float yPlane = 0;
	float zPlane = 0;

	if (Input::Get().isKeyPressed(KeyCode::S))
		zPlane = -0.1f;
	if (Input::Get().isKeyPressed(KeyCode::W))
		zPlane = 0.1f;
	if (Input::Get().isKeyPressed(KeyCode::A))
		xPlane = 0.1f;
	if (Input::Get().isKeyPressed(KeyCode::D))
		xPlane = -0.1f;
	if (Input::Get().isKeyPressed(KeyCode::Q))
		yPlane = 0.1f;
	if (Input::Get().isKeyPressed(KeyCode::E))
		yPlane = -0.1f;

	if (Input::Get().isButtonClicked(Button1))
	{
		Vector2 delta = Vector2(float(Input::Get().GetMouseX()) * ROTATION_GAIN, float(Input::Get().GetMouseY()) * ROTATION_GAIN) - lastMouse;
		lastMouse = Vector2(float(Input::Get().GetMouseX()) * ROTATION_GAIN, float(Input::Get().GetMouseY()) * ROTATION_GAIN);
		m_pitch -= delta.GetY();
		m_yaw += delta.GetX();

		float limit = DirectX::XM_PI / 2.0f - 0.01f;

		m_pitch = std::max(-limit, m_pitch);
		m_pitch = std::min(limit, m_pitch);
	}
	else
	{
		lastMouse = Vector2(float(Input::Get().GetMouseX()) * ROTATION_GAIN, float(Input::Get().GetMouseY()) * ROTATION_GAIN);
	}

	float y = sinf(m_pitch);
	float r = cosf(m_pitch);
	float z = r * cosf(m_yaw);
	float x = r * sinf(m_yaw);

	move.SetX(move.GetX() + (zPlane * x + xPlane * -z));
	move.SetY(move.GetY() + (zPlane * y + yPlane));
	move.SetZ(move.GetZ() + (zPlane * z + xPlane * x));

	Vector3 look = Vector3(move.GetX() + x, move.GetY() + y, move.GetZ() + z);

	//mainCamera.SetPosition(move);
	mainCamera.SetEyeAtUp(move, look, Vector3(kYUnitVector));
}
