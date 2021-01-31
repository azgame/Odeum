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

void CollisionHandler::SphereSphereCollisionResponse(SphereCollider& sc1, SphereCollider& sc2, float e) {
	// Setup Variables
	float m1 = sc1.GetRigidbody()->GetMass();
	float m2 = sc2.GetRigidbody()->GetMass();

	Vector3 pos1 = sc1.GetPosition();
	Vector3 pos2 = sc2.GetPosition();

	Vector3 n = (pos2 - pos1).Normalize();

	Vector3 vi1 = Vector3(sc1.GetRigidbody()->GetVelocity());
	Vector3 vi2 = Vector3(sc2.GetRigidbody()->GetVelocity());

	Vector3 v = vi2 - vi1;

	float fellingSpeed = Math::Dot(v, n);

	// don't collide in this case
	if (fellingSpeed >= 0) {
		return;
	}

	float speed1 = (2 * m2 * fellingSpeed) / (m1 + m2);
	float speed2 = (fellingSpeed * (m2 - m1)) / (m1 + m2);
	sc1.GetRigidbody()->AddVelocity(Vector4(n * speed1));
	sc2.GetRigidbody()->AddVelocity(Vector4(n * (speed2 - fellingSpeed)));

	Vector3 wi1 = Vector3(sc1.GetRigidbody()->GetRotation());
	Vector3 wi2 = Vector3(sc2.GetRigidbody()->GetRotation());

	Vector3 wf1 = (Math::Cross(n, Math::Cross(n, wi1))) + Vector3(0, sc2.GetRigidbody()->GetVelocity().GetY(), sc2.GetRigidbody()->GetVelocity().GetZ());
	Vector3 wf2 = (Math::Cross(n, Math::Cross(n, wi2))) - Vector3(0, sc1.GetRigidbody()->GetVelocity().GetY(), sc1.GetRigidbody()->GetVelocity().GetZ());

	sc1.GetRigidbody()->SetAngularVelocity(Vector4(wf1));
	sc2.GetRigidbody()->SetAngularVelocity(Vector4(wf2));

	/*
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
	*/
	
	/*
	Vector3 U1x, U1y, U2x, U2y, V1x, V1y, V2x, V2y;


	float m1, m2, x1, x2;
	Vector3 v1temp, v1, v2, v1x, v2x, v1y, v2y, x(sc1.GetPosition()- sc2.GetPosition());

	x.Normalize();
	v1 = Vector3(sc1.GetRigidbody()->GetVelocity());
	x1 = Math::Dot(x, v1);
	v1x = x * x1;
	v1y = v1 - v1x;
	m1 = sc1.GetRigidbody()->GetMass();

	x = x * -1;
	v2 = Vector3(sc2.GetRigidbody()->GetVelocity());
	x2 = Math::Dot(x, v2);
	v2x = x * x2;
	v2y = v2 - v2x;
	m2 = sc2.GetRigidbody()->GetMass();

	sc1.GetRigidbody()->SetVelocity(Vector4(v1x * (m1 - m2) / (m1 + m2) + v2x * (2 * m2) / (m1 + m2) + v1y) / 12);
	sc2.GetRigidbody()->SetVelocity(Vector4(v1x * (2 * m1) / (m1 + m2) + v2x * (m2 - m1) / (m1 + m2) + v2y) / 12);
	*/
}

void CollisionHandler::SphereStaticBoxCollisionResponse(SphereCollider& sc, BoxCollider& bc)
{
}

void CollisionHandler::OBBOBBCollisionRespones(BoxCollider& bc1, BoxCollider& bc2)
{

}

void CollisionHandler::GJKCollisionResponse(ComplexCollider& cc1, ComplexCollider& cc2, Simplex<Vector3>& simplex)
{	
	CollisionPoints collisionPoints = Math::EPA(cc1.GetCollider(), cc2.GetCollider(), simplex);

	//Vector3 P = simplex.GetCentroid();

	if (collisionPoints.penetrationDepth < 0.01f)
		return;

	Vector3 n = collisionPoints.normal;
	// hardcoding coefficient of restitution for now - 0 is no bounce, 1 is super bounce
	float e = 0.8f;

	Vector3 vi1 = Vector3(cc1.GetRigidbody()->GetVelocity());
	Vector3 vi2 = Vector3(cc2.GetRigidbody()->GetVelocity());
	Vector3 wi1 = Vector3(cc1.GetRigidbody()->GetAngularVelocity());
	Vector3 wi2 = Vector3(cc2.GetRigidbody()->GetAngularVelocity());

	float m1 = cc1.GetRigidbody()->GetMass();
	float m2 = cc2.GetRigidbody()->GetMass();

	// want to try and find the point of intersection for more realistic 
	// i'm just finding the middle of the distance between the centers of each body for now
	// **** NEEDS TO BE CHANGED **** - this seems like the tough part

	Vector3 P = (cc1.GetPosition() + cc2.GetPosition()) / 2.0f;


	Vector3 r1 = P - cc1.GetPosition();
	Vector3 r2 = P - cc2.GetPosition();

	Vector3 vi12 = vi1 - vi2;

	// with inertia?
	float i1 = cc1.GetRigidbody()->GetMomentOfInertiaSphere();
	float i2 = cc2.GetRigidbody()->GetMomentOfInertiaSphere();

	//float j = (-(1.0f + e) * (Math::Dot(vi12, n))) / (Math::Dot(n, n) * ((1.0f / m1) + (1.0f / m2)));
	float j = (-(1.0f + e) * (Math::Dot(vi12, n))) / (Math::Dot(n, n) * ((1.0f / m1) + (1.0f / m2)) + (Math::Dot(r1, n) * Math::Dot(r1, n) / i1) + (Math::Dot(r2, n) * Math::Dot(r2, n) / i2));

	Vector3 vf1 = vi1 + n * (j / m1);
	Vector3 vf2 = vi2 - n * (j / m2);

	cc1.GetRigidbody()->SetVelocity(Vector4(vf1, 1.0f));
	cc2.GetRigidbody()->SetVelocity(Vector4(vf2, 1.0f));

	Vector3 wf1 = wi1 + Math::Cross(r1, n * j) / i1;
	Vector3 wf2 = wi2 - Math::Cross(r2, n * j) / i2;
	cc1.GetRigidbody()->SetAngularVelocity(Vector4(wf1, 1.0f));
	cc2.GetRigidbody()->SetAngularVelocity(Vector4(wf2, 1.0f));
}