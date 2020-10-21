#include "GameObject.h"

#include "../Rendering/DirectX12/SceneGraph.h"
#include "Component.h"

// Moving model loading to a graphics component
GameObject::GameObject(std::string fileName, std::string tag_)
{
	m_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_rotation = Vector4(kYUnitVector);
	m_scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	UpdateTransform(m_position, 0.0f, m_rotation, m_scale);

	tag = tag_;

	m_model.SetParent(this);
	m_model.Load(fileName);

	SceneGraph::Get()->AddGameObject(this);
}

GameObject::GameObject(ShapeTypes preDefinedShape, Colour colour, std::string tag_)
{
	m_position = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	m_rotation = Vector4(kYUnitVector);
	m_scale = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	tag = tag_;

	UpdateTransform(m_position, 0.0f, m_rotation, m_scale);

	m_model.SetParent(this);

	switch (preDefinedShape)
	{
	case ShapeTypes::CubeShape:
	{
		m_model.Load("Engine/Resources/Models/Cube.obj");
		break;
	}
	default:
		break;
	}

	SceneGraph::Get()->AddGameObject(this);
}

GameObject::~GameObject()
{
	for (int i = 0; i < m_components.size(); i++)
	{
		delete m_components[i];
	}

	m_components.clear();
}

void GameObject::Initialize(std::string modelTextureLoadFile)
{
	fileName = modelTextureLoadFile;
}

void GameObject::Update(float deltaTime)
{
	for (auto component : m_components)
		component->Update(deltaTime);
}

void GameObject::SetPosition(Vector4 position)
{
	m_position = position;
	UpdateTransform(m_position, 0.0f, m_rotation, m_scale);
}

void GameObject::SetRotation(Vector4 rotation, float angle)
{
	m_rotation = rotation;
	UpdateTransform(m_position, angle, m_rotation, m_scale);
}

void GameObject::SetScale(Vector4 scale)
{
	m_scale = scale;
	UpdateTransform(m_position, 0.0f, m_rotation, m_scale);
}

void GameObject::SetMass(float mass)
{
	m_mass = mass;
}

void GameObject::CreateAttachedComponent(Component* pAttachedComponent)
{
	// Attach T
	pAttachedComponent->OnAttach(this); // On attach allows implementations to define how they register to systems.  
											// eg. A component which inherits from GraphicsComponent will register with the scene graph
	m_components.push_back(pAttachedComponent);
}

void GameObject::UpdateTransform(Vector4 position, float angle, Vector4 rotation, Vector4 scale)
{
	m_modelMatrix = Matrix4(DirectX::XMMatrixScalingFromVector(scale.GetVec()) * DirectX::XMMatrixRotationAxis(rotation.GetVec(), angle) * DirectX::XMMatrixTranslationFromVector(position.GetVec()));
	// update bounding box
	bbox.basis = Matrix3(m_modelMatrix);
}

void GameObject::UpdateTransform(Vector4 position, Quaternion rotationQuat, Vector4 scale)
{
	m_modelMatrix = Matrix4(DirectX::XMMatrixScalingFromVector(scale.GetVec()) * GetRotationMatrix(rotationQuat) * DirectX::XMMatrixTranslationFromVector(position.GetVec()));
	// update bounding box
	bbox.basis = Matrix3(m_modelMatrix);
}

Matrix4 GameObject::GetRotationMatrix(Quaternion quat)
{
	
	// https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
	Vector4 q = quat.GetVector4();

	Matrix4 m;
	
	// first row
	//m.GetX().SetX(1 - (2 * q.GetY() * q.GetY()) - (2 * q.GetZ() * q.GetZ())); // 1 - 2*y2 - 2*z2
	//m.GetX().SetY((2 * q.GetX() * q.GetY()) - (2 * q.GetZ() * q.GetW()));
	//m.GetX().SetZ((2 * q.GetX() * q.GetZ()) + (2 * q.GetY() * q.GetW()));
	// first row 
	m.SetX(Vector4(1 - (2 * q.GetY() * q.GetY()) - (2 * q.GetZ() * q.GetZ()), // [0][0]
				  (2 * q.GetX() * q.GetY()) - (2 * q.GetZ() * q.GetW()),      // [0][1]
				  (2 * q.GetX() * q.GetZ()) + (2 * q.GetY() * q.GetW()),      // [0][2]
				  0.0f));													  // [0][3]

	// second row
	//m.GetY().SetX((2 * q.GetX() * q.GetY()) + (2 * q.GetZ() * q.GetW()));
	//m.GetY().SetY(1 - (2 * q.GetX() * q.GetX()) - (2 * q.GetZ() * q.GetZ()));
	//m.GetY().SetZ((2 * q.GetY() * q.GetZ()) - (2 * q.GetX() * q.GetW()));
	// second row
	m.SetY(Vector4((2 * q.GetX() * q.GetY()) + (2 * q.GetZ() * q.GetW()),      // [1][0]
					1 - (2 * q.GetX() * q.GetX()) - (2 * q.GetZ() * q.GetZ()), // [1][1]
					(2 * q.GetY() * q.GetZ()) - (2 * q.GetX() * q.GetW()),     // [1][2]
					0.0f));													   // [1][3]

	// third row
	//m.GetZ().SetX((2 * q.GetX() * q.GetZ()) - (2 * q.GetY() * q.GetW()));
	//m.GetZ().SetY((2 * q.GetY() * q.GetZ()) + (2 * q.GetX() * q.GetW()));
	//m.GetZ().SetZ(1 - (2 * q.GetX() * q.GetX()) - (2 * q.GetY() * q.GetY()));
	//third row
	m.SetZ(Vector4((2 * q.GetX() * q.GetZ()) - (2 * q.GetY() * q.GetW()),     // [2][0]
				   (2 * q.GetY() * q.GetZ()) + (2 * q.GetX() * q.GetW()),     // [2][1]
				   1 - (2 * q.GetX() * q.GetX()) - (2 * q.GetY() * q.GetY()), // [2][2]
				   0.0f));													  // [2][3]
	
	m.SetW(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	return m;	
}
