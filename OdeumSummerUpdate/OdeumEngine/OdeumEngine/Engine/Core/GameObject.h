#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../pch.h"
#include "../Rendering/DirectX12/Model.h"
#include "../Rendering/DirectX12/Colour.h"

#include "Component.h"

class GameObject
{
public:
	GameObject(std::string fileName, ShapeTypes preDefinedShape = ShapeTypes::NoShape, Colour colour = Colour(1.0f, 1.0f, 1.0f, 1.0f)); // temp
	~GameObject();

	void Update();

	template<typename T>
	void AddComponent();

	template<typename... Ts>
	void AddComponents();

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

	void CreateAttachedComponent(Component** attachedComponent);
	void UpdateTransform(Vector4 position, float angle, Vector4 rotation, Vector4 scale);
};

template<typename T>
inline void GameObject::AddComponent<T>()
{
	// Verify T is of type Component
	// Verify type of T is not already an attached component
	// Allocate T and register to systems as necessary
	// Attach T
}

template<typename ...Ts>
inline void GameObject::AddComponents()
{
	Debug::Warning("Parameter packed multiple component attachment not supported yet", __FILENAME__, __LINE__);
	throw std::runtime_error("Parameter packed multiple component attachment not supported yet");
}

#endif