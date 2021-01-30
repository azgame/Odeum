#include "SceneObjects.h"

std::vector<GameObject*> SceneObjects::sm_sceneGameObjects;
std::mutex SceneObjects::sm_mutex;
std::unique_ptr<SceneObjects> SceneObjects::sm_instance = nullptr;

void SceneObjects::AddGameObject(GameObject* go)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	sm_sceneGameObjects.push_back(go);
}

void SceneObjects::RemoveGameObject(GameObject* go)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	auto it = std::find(sm_sceneGameObjects.begin(), sm_sceneGameObjects.end(), go);
	if (it != sm_sceneGameObjects.end())
		sm_sceneGameObjects.erase(it);
}

void SceneObjects::UpdateObjects(float deltaTime)
{
	for (auto go : sm_sceneGameObjects)
		go->Update(deltaTime);
}

void SceneObjects::Uninitialize()
{
	for (auto go : sm_sceneGameObjects)
		SAFE_DELETE(go);

	sm_sceneGameObjects.clear();
}
