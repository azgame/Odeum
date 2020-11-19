#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "../../../pch.h"
#include "../../Core/GameObject.h"

class SceneGraph
{
public:
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator=(const SceneGraph&) = delete;
	SceneGraph& operator=(SceneGraph&&) = delete;

	static SceneGraph* Get() 
	{ 
		if (sm_instance.get() == nullptr)
			sm_instance.reset(new SceneGraph);
		return sm_instance.get();
	}

	void AddGameObject(GameObject* go);
	void RemoveGameObject(GameObject* go);
	void LoadObjectsIntoMemory();
	void Uninitialize();

	inline std::vector<GameObject*>& GetGameObjects() 
	{
		return sm_sceneGameObjects; 
	}

private:

	SceneGraph() {}
	~SceneGraph() {}

	static std::unique_ptr<SceneGraph> sm_instance;
	friend std::default_delete<SceneGraph>;

	static std::vector<GameObject*> sm_sceneGameObjects;
	static std::mutex sm_mutex;
};

#endif