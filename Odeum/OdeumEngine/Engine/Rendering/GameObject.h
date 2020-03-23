#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"

class GameObject
{
public:
	GameObject(Model* model_);
	~GameObject();

	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Render(ID3D12GraphicsCommandList* commandList_);

	Model* GetModel() { return m_model; }

	DirectX::XMFLOAT4 position;
private:
	Model* m_model;
};

#endif