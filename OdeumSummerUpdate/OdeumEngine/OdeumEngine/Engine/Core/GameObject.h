#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../pch.h"
#include "../Rendering/DirectX12/Model.h"

class GameObject
{
public:
	GameObject(std::string fileName, ShapeTypes preDefinedShape = ShapeTypes::NoShape);
	~GameObject();

	void Update();

	Model& GetModel() { return m_model; }
	const Matrix4 GetTransform() const { return Matrix4(DirectX::XMMatrixTranspose(m_modelMatrix)); }

	Vector4 GetPosition() { return m_position; }
	Vector4 GetVelocity() { return m_velocity; }
	Vector4 GetRotation() { return m_rotation; }
	Vector4 GetScale() { return m_scale; }
	
	void SetPosition(Vector4 position);
	void SetRotation(Vector4 rotation, float angle = 0.0f);
	void SetScale(Vector4 scale);

private:
	Model m_model;
	Matrix4 m_modelMatrix;

	Vector4 m_position;
	Vector4 m_velocity;
	Vector4 m_rotation;
	Vector4 m_scale;

	void UpdateTransform(Vector4 position, float angle, Vector4 rotation, Vector4 scale);
};

#endif