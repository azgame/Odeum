#include "CameraController.h"
#include "OdeumEngine.h"
#include "Camera.h"
#include "../Math/D3DMath.h"

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

	move.SetX(move.GetX() + -zPlane + xPlane);
	move.SetY(move.GetY() + -zPlane + yPlane);
	move.SetZ(move.GetZ() + zPlane + xPlane);

	mainCamera.SetPosition(move);
}
