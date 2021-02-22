#include "SceneObjects.h"

#define PERSISTENT 0

uint16_t SceneObjects::sm_currentScene;
std::unordered_map<uint16_t, std::vector<GameObject*>> SceneObjects::sm_sceneObjects;
std::mutex SceneObjects::sm_mutex;
std::unique_ptr<SceneObjects> SceneObjects::sm_instance = nullptr;

void SceneObjects::AddGameObject(GameObject* go)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	if (go->IsPersistent())
		sm_sceneObjects[PERSISTENT].push_back(go);
	else
		sm_sceneObjects[sm_currentScene].push_back(go);
}

void SceneObjects::RemoveGameObject(GameObject* go)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	if (go->IsPersistent())
	{
		auto it = std::find(sm_sceneObjects[PERSISTENT].begin(), sm_sceneObjects[PERSISTENT].end(), go);
		if (it != sm_sceneObjects[PERSISTENT].end())
			sm_sceneObjects[PERSISTENT].erase(it);
	}
	else
	{
		auto it = std::find(sm_sceneObjects[sm_currentScene].begin(), sm_sceneObjects[sm_currentScene].end(), go);
		if (it != sm_sceneObjects[sm_currentScene].end())
			sm_sceneObjects[sm_currentScene].erase(it);
	}
}

void SceneObjects::InitializeObjects()
{
	for (auto go : sm_sceneObjects[PERSISTENT])
		go->OnStart();
	
	for (auto go : sm_sceneObjects[sm_currentScene])
		go->OnStart();
}

void SceneObjects::UpdateObjects(float deltaTime)
{
	for (auto go : sm_sceneObjects[PERSISTENT])
		go->Update(deltaTime);

	for (auto go : sm_sceneObjects[sm_currentScene])
		go->Update(deltaTime);
}

void SceneObjects::UnloadCurrentScene()
{
	for (auto go : sm_sceneObjects[sm_currentScene])
		SAFE_DELETE(go);

	sm_sceneObjects[sm_currentScene].clear();
}

void SceneObjects::Uninitialize()
{
	UnloadCurrentScene();

	for (auto go : sm_sceneObjects[PERSISTENT])
		SAFE_DELETE(go);

	sm_sceneObjects[PERSISTENT].clear();
}
