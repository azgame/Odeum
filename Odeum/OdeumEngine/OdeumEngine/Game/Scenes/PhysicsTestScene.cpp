#include "PhysicsTestScene.h"
#include "../Components/Rigidbody.h"

PhysicsTestScene::PhysicsTestScene()
{
	// First Cube -- Hardcoding collider for now, will move with visual gameobject
	go_1 = new GameObject(CubeShape, Colour(1.0f, 0.0f, 0.0f));
	go_2 = new GameObject(CubeShape, Colour(0.9f, 0.1f, 0.1f));

	go_1->AddComponent<Rigidbody>();
	go_2->AddComponent<Rigidbody>();

	go_1->GetComponent<Rigidbody>()->SetPosition(Vector4(2.0f, 0.0f, 0.0f, 0.0f));
	go_2->GetComponent<Rigidbody>()->SetPosition(Vector4(-2.0f, 0.0f, 0.0f, 0.0f));

	std::vector<Vector3> c1;
	c1.push_back(Vector3(-1.0f, -1.0f, -1.0f));
	c1.push_back(Vector3(-1.0f, -1.0f, 1.0f));
	c1.push_back(Vector3(-1.0f, 1.0f, -1.0f));
	c1.push_back(Vector3(-1.0f, 1.0f, 1.0f));
	c1.push_back(Vector3(1.0f, -1.0f, -1.0f));
	c1.push_back(Vector3(1.0f, -1.0f, 1.0f));
	c1.push_back(Vector3(1.0f, 1.0f, -1.0f));
	c1.push_back(Vector3(1.0f, 1.0f, 1.0f));

	// Second Tetrahedron
	std::vector<Vector3> c2;
	c2.push_back(Vector3(-1.0f, -1.0f, -1.0f));
	c2.push_back(Vector3(-1.0f, -1.0f, 1.0f));
	c2.push_back(Vector3(-1.0f, 1.0f, -1.0f));
	c2.push_back(Vector3(-1.0f, 1.0f, 1.0f));
	c2.push_back(Vector3(1.0f, -1.0f, -1.0f));
	c2.push_back(Vector3(1.0f, -1.0f, 1.0f));
	c2.push_back(Vector3(1.0f, 1.0f, -1.0f));
	c2.push_back(Vector3(1.0f, 1.0f, 1.0f));

	c_1 = new Collider(c1);
	c_2 = new Collider(c2);

	std::cout << "FIRST CUBE:" << std::endl;
	c_1->Print();
	std::cout << std::endl;

	std::cout << "SECOND CUBE:" << std::endl;
	c_2->Print();
	std::cout << std::endl;

	
	// Box Collision with GJK
	isMoving = false;
	isRotating = false;
	isRotationalVelocity = false;
	rotationCounter = 0.0f;
}

PhysicsTestScene::~PhysicsTestScene()
{
	delete c_1;
	c_1 = nullptr;

	delete c_2;
	c_2 = nullptr;

	delete go_1;
	go_1 = nullptr;
	
	delete go_2;
	go_2 = nullptr;
}

bool PhysicsTestScene::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);
	return true;
}

void PhysicsTestScene::Update(const float deltaTime_)
{
	if (isMoving)
	{
		go_1->GetComponent<Rigidbody>()->Transpose(Vector4(-0.0005f, 0.0f, 0.0f, 0.0f));
		c_1->Transpose(Vector3(go_1->GetComponent<Rigidbody>()->GetPosition()));
		go_2->GetComponent<Rigidbody>()->Transpose(Vector4(0.0005f, 0.0f, 0.0f, 0.0f));
		c_2->Transpose(Vector3(go_2->GetComponent<Rigidbody>()->GetPosition()));
	}
	if (isRotating)
	{
		go_1->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 0.0f, 1.0f, 0.0f), 65.0f);
		c_1->Rotate(Vector3(0.0f, 0.0f, 1.0f), 65.0f);
		go_2->GetComponent<Rigidbody>()->SetRotation(Vector4(0.0f, 0.0f, 1.0f, 0.0f), 45.0f);
		c_2->Rotate(Vector3(0.0f, 0.0f, 1.0f), 45.0f);
		isRotating = false;
		//rotationCounter += 0.005f;
	}
	if (isRotationalVelocity)
	{
		go_1->GetComponent<Rigidbody>()->SetVelocity(Vector4(-0.005f, 0.0f, 0.0f, 0.0f));
		go_1->GetComponent<Rigidbody>()->AddAngularVelocity(Vector4(0.0f, 0.0f, 2.0f, 0.0f), 1.0f);
		go_2->GetComponent<Rigidbody>()->SetVelocity(Vector4(0.005f, 0.0f, 0.0f, 0.0f));
		go_2->GetComponent<Rigidbody>()->AddAngularVelocity(Vector4(0.0f, 1.0f, 1.0f, 0.0f), 0.001f);
	}

	if (CollisionDetection::GJKCollisionDetection(c_1, c_2))
	{
		std::cout << "COLLISION DETECTED!!" << std::endl;
	}
	else
	{
		std::cout << "DID NOT COLLIDE!!" << std::endl;
	}

	go_1->Update(deltaTime_);
	go_2->Update(deltaTime_);
}

void PhysicsTestScene::UIRender()
{
	ImGui::Begin("Game UI");
	if (ImGui::Button("Reset Position"))
	{
		go_1->GetComponent<Rigidbody>()->SetPosition(Vector4(2.0f, 0.0f, 0.0f, 0.0f));
		go_2->GetComponent<Rigidbody>()->SetPosition(Vector4(-2.0f, 0.0f, 0.0f, 0.0f));

		go_1->GetComponent<Rigidbody>()->SetVelocity(Vector4());
		go_2->GetComponent<Rigidbody>()->SetVelocity(Vector4());

		go_1->GetComponent<Rigidbody>()->SetRotation(Vector4(1.0f, 0.0f, 0.0f, 0.0f), 0.0f);
		go_2->GetComponent<Rigidbody>()->SetRotation(Vector4(1.0f, 0.0f, 0.0f, 0.0f), 0.0f);

		isMoving = false;
		isRotating = false;
		isRotationalVelocity = false;
	}

	if (ImGui::Button("Move Together"))
	{
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