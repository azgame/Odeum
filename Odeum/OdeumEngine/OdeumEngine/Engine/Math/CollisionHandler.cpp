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

bool CollisionHandler::SphereSphereCollisionDetection(SphereCollider sc1, SphereCollider sc2)
{
	float distance = (sc1.GetPosition() - sc2.GetPosition()).Mag();

	float sumOfRadius = sc1.GetRadius() + sc2.GetRadius();

	if (distance < sumOfRadius)
	{
		return true;
	}
	else {
		return false;
	}
}

void CollisionHandler::SphereSphereCollisionResponse(SphereCollider& sc1, SphereCollider& sc2, float e) {
	// Setup Variables
	float m1 = sc1.GetRigidbody()->GetMass();
	float m2 = sc2.GetRigidbody()->GetMass();

	Vector3 pos1 = sc1.GetPosition();
	Vector3 pos2 = sc2.GetPosition();

	Vector3 n = pos2 - pos1;

	Vector3 vi1 = Vector3(sc1.GetRigidbody()->GetVelocity());
	Vector3 vi2 = Vector3(sc2.GetRigidbody()->GetVelocity());

	//Quaternion wi1 = sc1.GetRigidbody()->GetOrientation();
	Vector3 wi1 = Vector3(sc1.GetRigidbody()->GetRotation());
	//Quaternion wi2 = sc2.GetRigidbody()->GetOrientation();
	Vector3 wi2 = Vector3(sc2.GetRigidbody()->GetRotation());

	// linear velocity
	Vector3 vf1 = (m1 * vi1 + m2 * vi2 + m2 * e * (vi2 - vi1)) / (m1 + m2);
	Vector3 vf2 = (m1 * vi1 + m2 * vi2 - m2 * e * (vi2 - vi1)) / (m1 + m2);

	// Angular Velocity
	Vector3 wf1 = (Math::Cross(n, Math::Cross(n, wi1))) + Vector3(0, vf2.GetY(), vf2.GetZ());
	Vector3 wf2 = (Math::Cross(n, Math::Cross(n, wi2))) - Vector3(0, vf1.GetY(), vf1.GetZ());

	// Update Spheres
	sc1.GetRigidbody()->SetVelocity(Vector4(vf1));
	sc1.GetRigidbody()->SetAngularVelocity(Vector4(wf1));
	sc2.GetRigidbody()->SetVelocity(Vector4(vf2));
	sc2.GetRigidbody()->SetAngularVelocity(Vector4(wf2));
}
