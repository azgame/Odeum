#include "SceneGraph.h"

std::vector<GameObject*> SceneGraph::sm_sceneGameObjects;
std::mutex SceneGraph::sm_mutex;
std::unique_ptr<SceneGraph> SceneGraph::sm_instance = nullptr;

void SceneGraph::AddGameObject(GameObject* go)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	sm_sceneGameObjects.push_back(go);
}

void SceneGraph::RemoveGameObject(GameObject* go)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	auto it = std::find(sm_sceneGameObjects.begin(), sm_sceneGameObjects.end(), go);
	if (it != sm_sceneGameObjects.end())
		sm_sceneGameObjects.erase(it);
}

void SceneGraph::LoadObjectsIntoMemory()
{
	for (auto go : sm_sceneGameObjects)
		go->GetModel().Load();
}

void SceneGraph::Uninitialize()
{
	for (auto go : sm_sceneGameObjects)
		SAFE_DELETE(go);

	sm_sceneGameObjects.clear();
}
