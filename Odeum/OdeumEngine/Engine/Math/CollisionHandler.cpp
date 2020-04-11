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

void CollisionHandler::MouseUpdate(DirectX::Mouse::State& mouse)
{
	D3D12_VIEWPORT viewport = OdeumEngine::GetInstance()->GetCamera()->GetViewPort();
	Ray mouseRay = CollisionDetection::ScreenPosToWorldRay(DirectX::XMFLOAT2(mouse.x, mouse.y), DirectX::XMFLOAT2(viewport.Width, viewport.Height), OdeumEngine::GetInstance()->GetCamera());

	if (m_scenePartition == nullptr) return;
	
	GameObject* hitResult = m_scenePartition->GetCollision(mouseRay);

	if (hitResult)
		hitResult->SetHit(true, mouse);

	for (auto c : previousCollisions)
	{
		if (c != hitResult && c != nullptr)
			c->SetHit(false, mouse);

		c = nullptr;
	}
	previousCollisions.clear();

	if (hitResult)
		previousCollisions.push_back(hitResult);
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