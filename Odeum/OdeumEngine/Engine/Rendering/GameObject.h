#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"
#include <string>

class GameObject
{
public:
	GameObject(Model* model_);
	~GameObject();

	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Update(float deltaTime);
	void Render(ID3D12GraphicsCommandList* commandList_);

	Model* GetModel() { return m_model; }

	DirectX::XMFLOAT4 position;

	std::string tag;
private:
	Model* m_model;
};

#endif