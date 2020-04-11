#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"
#include <string>

class OctNode;

class GameObject
{
public:
	GameObject(Model* model_, DirectX::XMFLOAT3 pos_);
	~GameObject();

	bool Initialize(ID3D12Device*, ID3D12GraphicsCommandList*);
	void Update(float deltaTime);
	void Render(ID3D12GraphicsCommandList* commandList_);

	Model* GetModel() { return m_model; }
	BoundingBox GetBoundingBox() { return m_box; }
	void SetHit(bool hit_, DirectX::Mouse::State mouse_);
	
	float movX;
	bool hit;
	
	std::string tag;

	DirectX::XMFLOAT3 GetPosition() { return position; }
	float GetAngle() { return angle; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }

	void SetPosition(DirectX::XMFLOAT3 position_);
	void SetAngle(float angle_);
	void SetRotation(DirectX::XMFLOAT3 rotation_);
	void SetScale(DirectX::XMFLOAT3 scale_);

private:
	DirectX::XMFLOAT3		position;
	DirectX::XMFLOAT3		rotation;
	DirectX::XMFLOAT3		scale;
	float					angle;
	Model*					m_model;
	BoundingBox				m_box;
	int						m_modelInstance;
};

#endif