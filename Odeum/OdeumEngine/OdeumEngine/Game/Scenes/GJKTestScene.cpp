#include "GJKTestScene.h"

GJKTestScene::GJKTestScene()
{
	// First Tetrahedron
	std::vector<Vector3> t1;
	t1.push_back(Vector3(0.5f, 0.8f, 0.3f));
	t1.push_back(Vector3(0.0f, 0.0f, 0.0f));
	t1.push_back(Vector3(0.5f, 0.0f, 0.9f));
	t1.push_back(Vector3(1.0f, 0.0f, 0.0f));

	// Second Tetrahedron
	std::vector<Vector3> t2;
	t2.push_back(Vector3(0.5f, 0.1f, 0.3f));
	t2.push_back(Vector3(0.0f, -0.7f, 0.0f));
	t2.push_back(Vector3(0.5f, -0.7f, 0.9f));
	t2.push_back(Vector3(1.0f, -0.7f, 0.0f));

	c1 = new Collider(t1);
	c2 = new Collider(t2);

	std::cout << "FIRST TETRAHEDRON:" << std::endl;
	c1->Print();
	std::cout << std::endl;

	std::cout << "SECOND TETRAHEDRON:" << std::endl;
	c2->Print();
	std::cout << std::endl;

	/*if (CollisionDetection::GJKCollisionDetection(c1, c2))
	{
		std::cout << "COLLISION DETECTED!!" << std::endl;
	}
	else
	{
		std::cout << "DID NOT COLLIDE!!" << std::endl;
	}
	*/
	// Box Collision with GJK

}

GJKTestScene::~GJKTestScene()
{
	delete c1;
	c1 = nullptr;

	delete c2;
	c2 = nullptr;
}

bool GJKTestScene::Initialize()
{
	Debug::Info("Creating Game Scene", __FILENAME__, __LINE__);
	return true;
}

void GJKTestScene::Update(const float deltaTime_)
{
}

void GJKTestScene::UIRender()
{
	ImGui::Begin("Game UI");
	if (ImGui::Button("Reset Position"))
	{
	}
	ImGui::End();
}