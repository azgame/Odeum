#include "SceneGraph.h"

std::queue<RenderComponent*> SceneGraph::sm_unloadedObjects;
std::vector<RenderComponent*> SceneGraph::sm_sceneGameObjects;
std::mutex SceneGraph::sm_mutex;
std::unique_ptr<SceneGraph> SceneGraph::sm_instance = nullptr;

void SceneGraph::AddRenderObject(RenderComponent* go)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	sm_unloadedObjects.push(go);
}

void SceneGraph::RemoveRenderObject(RenderComponent* go)
{
	std::lock_guard<std::mutex> lg(sm_mutex);

	auto it = std::find(sm_sceneGameObjects.begin(), sm_sceneGameObjects.end(), go);
	if (it != sm_sceneGameObjects.end())
		sm_sceneGameObjects.erase(it);
}

void SceneGraph::LoadGraphicsObjects()
{
	while (!sm_unloadedObjects.empty())
	{
		auto go = sm_unloadedObjects.front(); sm_unloadedObjects.pop();

		if (!go->GetModel().isLoaded)
			go->GetModel().Load();

		sm_sceneGameObjects.push_back(go);
	}
}

void SceneGraph::Uninitialize()
{
}
