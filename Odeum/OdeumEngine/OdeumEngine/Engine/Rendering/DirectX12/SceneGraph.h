#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "../../../pch.h"
#include "../../../Game/Components/RenderComponent.h"
#include <queue>

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

	void AddRenderObject(RenderComponent* go);
	void RemoveRenderObject(RenderComponent* go);
	void LoadGraphicsObjects();
	void Uninitialize();

	inline std::vector<RenderComponent*>& GetRenderObjects()
	{
		return sm_sceneGameObjects; 
	}

private:

	SceneGraph() {}
	~SceneGraph() {}

	static std::unique_ptr<SceneGraph> sm_instance;
	friend std::default_delete<SceneGraph>;

	static std::queue<RenderComponent*> sm_unloadedObjects;
	static std::vector<RenderComponent*> sm_sceneGameObjects;
	static std::mutex sm_mutex;
};

#endif