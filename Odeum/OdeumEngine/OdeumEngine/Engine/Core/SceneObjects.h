#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include "../../pch.h"
#include "GameObject.h"

#include <unordered_map>

/*		Scene loading

	On scene load ->	load gameobject references into dictionary, key=scene# value=list of gameobjects
						gameobjects flagged as persistent will be stored in the persistent loading area instead

	On scene exit ->	unload scene objects in same order they were loaded, let render components signal for renderer to unload themselves,
						let collider components signal for spatial partition to unload themselves. Verify unloading was successful, if so, load next scene
*/

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

	void UnloadCurrentScene();
	void Uninitialize();

	void SetCurrentScene(uint16_t SceneNum) { sm_currentScene = SceneNum; }

	inline std::vector<GameObject*>& GetGameObjects()
	{
		std::vector<GameObject*> returnObjects;
		returnObjects.reserve(sm_sceneObjects[0].size() + sm_sceneObjects[sm_currentScene].size());
		returnObjects.insert(returnObjects.end(), sm_sceneObjects[0].begin(), sm_sceneObjects[0].end());
		returnObjects.insert(returnObjects.end(), sm_sceneObjects[sm_currentScene].begin(), sm_sceneObjects[sm_currentScene].end());
		return returnObjects;
	}

private:
	SceneObjects() {}
	~SceneObjects() {}

	static uint16_t sm_currentScene;

	static std::unordered_map<uint16_t, std::vector<GameObject*>> sm_sceneObjects;

	static std::unique_ptr<SceneObjects> sm_instance;
	friend std::default_delete<SceneObjects>;
	static std::mutex sm_mutex;
};

#endif