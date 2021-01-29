#include "BenScene.h"

#include "../../Engine/Math/CollisionHandler.h"
#include "../../Engine/Math/Collider.h"
#include "../../Engine/Math/Plane.h"
#include "../../Engine/NavMesh/NavMeshManager.h"
#include "../../Engine/Rendering/DirectX12/ParticleManager.h"
#include "../../Engine/Rendering/DirectX12/SceneGraph.h"

#include "../Components/Rigidbody.h"

BenScene::BenScene() : Scene(), angle(0.0f), direction(1.0f)
{
	OdeumEngine::Get().GetCamera().SetPosition(Vector3(0.0f, 10.0f, -25.0f));	

	go1 = new GameObject(ShapeTypes::CubeShape, Colour(0.4, 0.7, 0.7));
	go1->AddComponent<Rigidbody>();
	go1->GetComponent<Rigidbody>()->SetPosition(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	go1->GetComponent<Rigidbody>()->SetRotation(Vector4(1.0f, 0.0f, 1.0f, 1.0f), 45.0f);
	//go1->GetComponent<Rigidbody>()->SetRadius(1.3f);
	//go1->GetComponent<Rigidbody>()->SetScale(Vector4(0.5f, 0.5, 0.5, 1.0f));
	go1->GetComponent<Rigidbody>()->SetVelocity(Vector4(0.0f, 0.2f, 0.0f, 1.0f));
	//go1->AddComponent<SphereCollider>();
	go1->AddComponent<ComplexCollider>();

	go2 = new GameObject(ShapeTypes::CubeShape, Colour(0.7, 0.4, 0.7));
	go2->AddComponent<Rigidbody>();
	go2->GetComponent<Rigidbody>()->SetPosition(Vector4(0.5f, 7.0f, 0.0f, 1.0f));
	go2->GetComponent<Rigidbody>()->SetRotation(Vector4(1.0f, 0.0f, 1.0f, 1.0f), -45.0f);
	//go2->GetComponent<Rigidbody>()->SetRadius(1.3f);
	//go2->GetComponent<Rigidbody>()->SetScale(Vector4(0.5f, 0.5, 0.5, 1.0f));
	go2->GetComponent<Rigidbody>()->SetVelocity(Vector4(0.0f, -0.2f, 0.0f, 1.0f));
	//go2->AddComponent<SphereCollider>();
	go2->AddComponent<ComplexCollider>();
	SceneGraph::Get()->LoadObjectsIntoMemory();


}

BenScene::~BenScene()
{
}

bool BenScene::Initialize()
{
	LOG("Creating Game Scene")
		return true;
}

void BenScene::Update(const float deltaTime_)
{
	cameraController.UpdateMainCamera();
	SceneGraph::Get()->UpdateObjects(deltaTime_);

	/*if (CollisionDetection::SphereSphereCollisionDetection(go1->GetComponent<SphereCollider>(), go2->GetComponent<SphereCollider>()))
	{
		CollisionHandler::GetInstance()->SphereSphereCollisionResponse(*go1->GetComponent<SphereCollider>(), *go2->GetComponent<SphereCollider>(), 1.0f);
	}*/

	if (CollisionDetection::GJKCollisionDetection(go1->GetComponent<ComplexCollider>(), go2->GetComponent<ComplexCollider>(), simplex))
	{
		std::cout << "COLLISION! ";
		CollisionHandler::GetInstance()->GJKCollisionResponse(*go1->GetComponent<ComplexCollider>(), *go2->GetComponent<ComplexCollider>(), simplex);
	}

}

void BenScene::UIRender()
{
	ImGui::Begin("Game UI");
	ImGui::Text("Enter game UI components here");
	ImGui::End();
}
