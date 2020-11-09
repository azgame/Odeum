#include "CollisionHandler.h"

#include "CollisionDetection.h"
#include "../Core/OdeumEngine.h"

std::unique_ptr<CollisionHandler> CollisionHandler::collisionInstance = nullptr;
std::vector<GameObject*> CollisionHandler::previousCollisions = std::vector<GameObject*>();
OctSpatialPartition* CollisionHandler::m_scenePartition = nullptr;

CollisionHandler::CollisionHandler()
{
	previousCollisions.reserve(10);
}

CollisionHandler::~CollisionHandler()
{
	Uninitialize();
}

CollisionHandler* CollisionHandler::GetInstance()
{
	if (collisionInstance.get() == nullptr) {
		collisionInstance.reset(new CollisionHandler);
	}
	return collisionInstance.get();
}

void CollisionHandler::Initialize(float worldsz_)
{
	m_scenePartition = new OctSpatialPartition(worldsz_);
}

void CollisionHandler::AddObject(GameObject* go_)
{
	m_scenePartition->AddObject(go_);
}

bool CollisionHandler::MouseCollide()
{
	if (Input::Get().isButtonClicked(Button1))
	{
		Ray mouseRay = GetMouseRay();

		GameObject* hit = RayGetFirstHit(mouseRay, nullptr);

		if (hit)
			return true;
	}

	return false;
}

Ray CollisionHandler::GetMouseRay()
{
	float width = (float)OdeumEngine::Get().GetWindow().GetWidth();
	float height = (float)OdeumEngine::Get().GetWindow().GetHeight();

	return CollisionDetection::ScreenPosToWorldRay(Vector2(Input::Get().GetMouseX(),
		Input::Get().GetMouseY()), Vector2(width, height),
		OdeumEngine::Get().GetCamera());
}

void CollisionHandler::RayQueryFirst(Ray& ray, Vector4* IntersectionPlane)
{
	if (m_scenePartition == nullptr) return;
	
	GameObject* hitResult = m_scenePartition->GetCollision(ray, IntersectionPlane);

	if (hitResult)
		hitResult->SetHit(true);

	for (auto c : previousCollisions)
	{
		if (c != hitResult && c != nullptr)
			c->SetHit(false);
	}

	previousCollisions.clear();

	if (hitResult)
		previousCollisions.push_back(hitResult);
}

void CollisionHandler::RayQueryList(Ray& ray, std::vector<GameObject*>& IntersectedObjects)
{
	if (m_scenePartition == nullptr) return;

	IntersectedObjects = m_scenePartition->GetCollisions(ray);
}

GameObject* CollisionHandler::RayGetFirstHit(Ray& ray, Vector4* IntersectionPlane)
{
	RayQueryFirst(ray, IntersectionPlane);
	
	// previous collisions should only have a single object in it if the ray intersected with anything
	if (previousCollisions.size() > 0)
		return previousCollisions[0];

	return nullptr;
}

std::vector<GameObject*>& CollisionHandler::RayGetList(Ray& ray)
{
	std::vector<GameObject*> collisions;

	RayQueryList(ray, collisions);

	return collisions;
}

void CollisionHandler::Update()
{
	m_scenePartition->UpdatePartition();
}

void CollisionHandler::Uninitialize()
{
	SAFE_DELETE(m_scenePartition);

	for (auto entry : previousCollisions)
		entry = nullptr;
	previousCollisions.clear();
}