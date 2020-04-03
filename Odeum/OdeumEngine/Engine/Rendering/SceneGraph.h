#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <memory>
#include <map>
#include <vector>
#include "GameObject.h"
#include "../Core/Camera.h"

#include "../../pch.h"

class SceneGraph
{
public:
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator=(const SceneGraph&) = delete;
	SceneGraph& operator=(SceneGraph&&) = delete;

	static SceneGraph* GetInstance();
	void Uninitialize();
	void AddModel(Model* model_);
	void AddGameObject(GameObject* go_, std::string tag_ = "");
	GameObject* GetGameObject(std::string tag_);

	void Update(const float deltaTime_);
	void Render(Camera* camera_);

private:
	SceneGraph();
	~SceneGraph();

	static std::unique_ptr<SceneGraph> sceneGraphInstance;
	friend std::default_delete<SceneGraph>;

	static std::map<UINT16, std::vector<Model*>> sceneModels;
	static std::map<std::string, GameObject*> sceneGameObjects;

};


#endif // !SCENEGRAPH_H
