#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../pch.h"
#include "../Rendering/DirectX12/Model.h"
#include "../Rendering/DirectX12/Colour.h"

class Component;

class GameObject
{
public:
	GameObject(std::string fileName); // temp
	GameObject(ShapeTypes preDefinedShape = ShapeTypes::NoShape, Colour colour = Colour(1.0f, 1.0f, 1.0f, 1.0f));
	~GameObject();

	void Initialize(std::string modelTextureLoadFile);
	virtual void Update(float deltaTime);

	template<typename T>
	void AddComponent();

	template<typename... Ts>
	void AddComponents();

	template<typename T>
	T* GetComponent();

	Model& GetModel() { return m_model; }
	const Matrix4 GetTransform() const { return Matrix4(DirectX::XMMatrixTranspose(m_modelMatrix)); }

	Vector4 GetPosition() { return m_position; }
	Vector4 GetVelocity() { return m_velocity; }
	Vector4 GetRotation() { return m_rotation; }
	Vector4 GetScale() { return m_scale; }
	float GetMass() { return m_mass; }
	
	void SetPosition(Vector4 position);
	void SetRotation(Vector4 rotation, float angle = 0.0f);
	void SetScale(Vector4 scale);
	void SetMass(float mass);

protected:

	void AddComponent();
	Model m_model;
	Matrix4 m_modelMatrix;

	std::string fileName;

	std::vector<Component*> m_components;

	Vector4 m_position;
	Vector4 m_velocity;
	Vector4 m_rotation;
	Vector4 m_scale;

	float m_mass;

	void CreateAttachedComponent(Component* pAttachedComponent);
	void UpdateTransform(Vector4 position, float angle, Vector4 rotation, Vector4 scale);
};

template<typename T>
inline void GameObject::AddComponent()
{
	// Verify T is of type Component, if it is not, it should throw an error, so its likely that the following assert will never be reached, but just in case...
	Component* component = new T();
	
	ASSERT(component != nullptr, "T must be of type Component!");

	// Verify type of T is not already an attached component
	bool exists = false;
	for (auto c : m_components)
		if (dynamic_cast<T*>(c)) exists = true;

	// Allocate T and register to systems as necessary
	if (!exists)
		CreateAttachedComponent(component);
	else
	{
		SAFE_DELETE(component);
		ERROR("Duplicate component attached");
		throw std::runtime_error("Component of this type has already been attached, we do not support duplicates");
	}

	component = nullptr;
}

template<typename ...Ts>
inline void GameObject::AddComponents()
{
	int unpacking[] = { 0, (AddComponent<Ts>(), 0)... }; // pack expansion trick. function call is a side effect of the initializer list being initialized to 0s, but is used for all va args, so we accomplish add component for all without recursively calling it
	(void)unpacking; // suppress unused variable warning
}

// Can lead to undefined behaviour with diamond inheritance structures. Please do not use diamond inheritance structures with components
template<typename T>
inline T* GameObject::GetComponent()
{
	for (auto c : m_components)
		if (dynamic_cast<T*>(c))
			return dynamic_cast<T*>(c);

	return nullptr;
}

#endif