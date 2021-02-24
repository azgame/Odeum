#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include "../../pch.h"
#include "GameObject.h"

class SceneObjects
{
public:
	SceneObjects(const SceneObjects&) = delete;
	SceneObjects(SceneObjects&&) = delete;
	SceneObjects& operator=(const SceneObjects&) = delete;
	SceneObjects& operator=(SceneObjects&&) = delete;

	static SceneObjects* Get()
	{
		if (sm_instance.get() == nullptr)
			sm_instance.reset(new SceneObjects);
		return sm_instance.get();
	}

	void AddGameObject(GameObject* go);
	void RemoveGameObject(GameObject* go);
	void InitializeObjects();
	void UpdateObjects(float deltaTime);

	void Uninitialize();

	inline std::vector<GameObject*>& GetGameObject()
	{
		return sm_sceneGameObjects;
	}

private:
	SceneObjects() {}
	~SceneObjects() {}

	static std::vector<GameObject*> sm_sceneGameObjects;

	static std::unique_ptr<SceneObjects> sm_instance;
	friend std::default_delete<SceneObjects>;
	static std::mutex sm_mutex;
};

#endif