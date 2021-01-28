#include "PhysicsTestScene2.h"
#include "../Components/Rigidbody.h"
#include "../Components/AudioSource.h"

#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

PhysicsTestScene2::PhysicsTestScene2()
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 15.0f, -20.0f));

	go_s = new GameObject(SphereShape, Colour(1.0f, 0.0f, 0.0f));
	go_a = new GameObject(SphereShape, Colour(0.0f, 0.0f, 1.0f));

	InitObjects();

	SceneGraph::Get()->LoadObjectsIntoMemory();

	SceneGraph::Get()->UpdateObjects(0.0167f);
}

PhysicsTestScene2::~PhysicsTestScene2()
{
	delete go_s;
	go_s = nullptr;

	delete go_a;
	go_a = nullptr;
}

bool PhysicsTestScene2::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);
	return true;
}

void PhysicsTestScene2::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();

	float distance = Vector3(go_s->GetComponent<Rigidbody>()->GetPosition() - go_a->GetComponent<Rigidbody>()->GetPosition()).Mag();
	float radius = (go_s->GetComponent<Rigidbody>()->GetRadius() + go_a->GetComponent<Rigidbody>()->GetRadius()) / 2.0f;

	if (distance < radius)
	{
		std::cout << "COLLISION" << std::endl;
		CollisionResponse();
	}

	SceneGraph::Get()->UpdateObjects(deltaTime_);
}

void PhysicsTestScene2::UIRender()
{
	ImGui::Begin("Game UI");
	if (ImGui::Button("Reset Position"))
	{
		go_s->GetComponent<Rigidbody>()->SetPosition(Vector4(kZero));
		go_a->GetComponent<Rigidbody>()->SetPosition(Vector4(-10.0f, 0.0f, 0.0f, 1.0f));

		go_s->GetComponent<Rigidbody>()->SetVelocity(Vector4());
		go_a->GetComponent<Rigidbody>()->SetVelocity(Vector4());

		isMoving = false;
		isRotating = false;
		isRotationalVelocity = false;
	}

	if (ImGui::Button("Move Together"))
	{
		// velocity
		go_a->GetComponent<Rigidbody>()->SetVelocity(Vector4(2.0f, 0.0f, 0.25f, 1.0f));

		isMoving = true;
	}


	if (ImGui::Button("Just Rotate"))
	{
		isRotating = true;
	}

	/*if (ImGui::Button("Rotational Velocity"))
	{
		isRotationalVelocity = true;
	}*/
	ImGui::End();
}

void PhysicsTestScene2::InitObjects() 
{
	// add rigidbody
	go_s->AddComponent<Rigidbody>();
	go_a->AddComponent<Rigidbody>();
	go_s->AddComponent<AudioSource>();
	
	// mass
	go_s->GetComponent<Rigidbody>()->SetMass(1.0f);
	go_a->GetComponent<Rigidbody>()->SetMass(1.0f);

	// position
	go_s->GetComponent<Rigidbody>()->SetPosition(Vector4(kZero));
	go_a->GetComponent<Rigidbody>()->SetPosition(Vector4(-10.0f, 0.0f, 0.0f, 1.0f));

	// velocity
	go_a->GetComponent<Rigidbody>()->SetVelocity(Vector4(2.0f, 0.0f, 0.25f, 1.0f));

	// radius
	go_s->GetComponent<Rigidbody>()->SetRadius(5.0f);
	go_a->GetComponent<Rigidbody>()->SetRadius(5.0f);

}

void PhysicsTestScene2::CollisionResponse() 
{
	// setup variables
	float e = 0.5f;
	float ms = go_s->GetComponent<Rigidbody>()->GetMass();
	float ma = go_a->GetComponent<Rigidbody>()->GetMass();
	Vector3 poss = Vector3(go_s->GetComponent<Rigidbody>()->GetPosition());
	Vector3 posa = Vector3(go_a->GetComponent<Rigidbody>()->GetPosition());
	Vector3 Vis = Vector3(go_s->GetComponent<Rigidbody>()->GetVelocity());
	Vector3 Via = Vector3(go_a->GetComponent<Rigidbody>()->GetVelocity());


	// inverse matrices
	Matrix3 Is = Matrix3(Vector3(1.0f/20.0f, 0.0f, 0.0f), Vector3(1.0f/40.0f, 0.0f, 0.0f), Vector3(1.0f/20.0f, 0.0f, 0.0f));
	Matrix3 Ia = Matrix3(Vector3(10.0f, 0.0f, 0.0f), Vector3(10.0f, 0.0f, 0.0f), Vector3(10.0f, 0.0f, 0.0f));

	Vector3 n = Vector3(2.0f/3.0f, 1.0f/3.0f, 2.0f/3.0f);

	// get the point of interesction between two spheres:
	// get the normalized direction from the ship to the asteroid and multiply the ship's radius by that direction vector
	Vector3 p = Vector3(posa - poss);
	p = p.Normalize();
	p = go_s->GetComponent<Rigidbody>()->GetRadius() * p;

	Vector3 rs = p - poss;
	Vector3 ra = p - posa;

	float Vr = Math::Dot(Vector3(n), Vector3(Vis - Via));

	// equation for J
	float J = (-Vr * (e + 1)) / ((1 / ms) + (1 / ma) + (Math::Dot(n, Is * (Math::Cross(Math::Cross(rs, n), rs)))) + (Math::Dot(n, Ia * (Math::Cross(Math::Cross(ra, n), ra)))));

	// linear velocity
	Vector3 Vfs = Vis + Vector3(J * n / ms);
	Vector3 Vfa = Via + Vector3(-J * n / ma);

	// angular velocity
	

	// update game objects
	go_s->GetComponent<Rigidbody>()->SetVelocity(Vector4(Vfs));
	go_a->GetComponent<Rigidbody>()->SetVelocity(Vector4(Vfa));
}