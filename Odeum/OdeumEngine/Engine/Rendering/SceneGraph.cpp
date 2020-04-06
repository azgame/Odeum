#include "SceneGraph.h"
#include "../Math/CollisionHandler.h"

std::unique_ptr<SceneGraph> SceneGraph::sceneGraphInstance = nullptr;
std::map<DxShaderProgram*, std::vector<Model*>> SceneGraph::sceneModels = std::map<DxShaderProgram*, std::vector<Model*>>();
std::map<std::string, GameObject*> SceneGraph::sceneGameObjects = std::map<std::string, GameObject*>();

SceneGraph::~SceneGraph()
{
	//Uninitialize();
}

SceneGraph::SceneGraph() 
{
}

SceneGraph* SceneGraph::GetInstance()
{
	if (sceneGraphInstance.get() == nullptr) {
		sceneGraphInstance.reset(new SceneGraph);
	}
	return sceneGraphInstance.get();
}

void SceneGraph::Uninitialize()
{
	if (sceneGameObjects.size() > 0) {
		for (auto go : sceneGameObjects) {
			delete go.second;
			go.second = nullptr;
		}
		sceneGameObjects.clear();
	}

	if (sceneModels.size() > 0) {
		for (auto entry : sceneModels) {
			if (entry.second.size() > 0) {
				for (auto model : entry.second) {
					delete model;
					model = nullptr;
				}
				entry.second.clear();
			}
		}
		sceneModels.clear();
	}
}

void SceneGraph::AddModel(Model* model_)
{
	if (sceneModels.find(model_->GetShaderProgram()) == sceneModels.end()) 
	{
		std::vector<Model*> tmp = std::vector<Model*>();
		tmp.reserve(10);
		sceneModels.insert(std::pair<DxShaderProgram*, std::vector<Model*>>(
			model_->GetShaderProgram(), tmp));
	}
	else {
		sceneModels[model_->GetShaderProgram()].push_back(model_);
	}
}

void SceneGraph::AddGameObject(GameObject* go_, std::string tag_)
{
	if (tag_ == "") 
	{
		std::string newTag = "GameObject" + std::to_string(sceneGameObjects.size() + 1);
		go_->tag = tag_;
		sceneGameObjects[newTag] = go_;
	}
	else if (sceneGameObjects.find(tag_) == sceneGameObjects.end()) 
	{
		go_->tag = tag_;
		sceneGameObjects[tag_] = go_;
	}
	else 
	{
		Debug::Error("Trying to add a GameObject with a tag " + tag_ + " that already exists"
			, __FILENAME__, __LINE__);
		std::string newTag = "GameObject" + std::to_string(sceneGameObjects.size() + 1);
		go_->tag = newTag;
		sceneGameObjects[newTag] = go_;
	}
	CollisionHandler::GetInstance()->AddObject(go_);
}

GameObject* SceneGraph::GetGameObject(std::string tag_)
{
	if (sceneGameObjects.find(tag_) != sceneGameObjects.end()) 
		return sceneGameObjects[tag_];

	return nullptr;
}

void SceneGraph::Update(const float deltaTime_)
{
	for (auto go : sceneGameObjects)
		go.second->Update(deltaTime_);
}

// Can't fully implement this yet, needs more work moving rendering into models/meshes
void SceneGraph::Render(Camera* camera_)
{
	/*for (auto entry : sceneModels) 
	{
		
		for (auto m : entry.second)
			m->Render(camera_);
	}*/
}
