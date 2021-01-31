#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"
#include "../../Engine/Rendering/DirectX12/Model.h"
#include "../../Engine/Math/BoundingBox.h"

class RenderComponent : public Component
{
public:
	
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	void Update(float deltaTime) override;

	void LoadModelFromFile(std::string fileName);
	void LoadShape(ShapeTypes preDefinedShape, Colour colour = Colour(1.0f, 1.0f, 1.0f, 1.0f));
	void CreateFromExistingModel(Model* Model);

	Model& GetModel() { return m_model; }

	OrientedBoundingBox& GetBoundingBox() { return modelOBB; }
	void SetBoundingBox(Vector3 Min, Vector3 Max) { modelOBB = OrientedBoundingBox(Min, Max); }

	bool IsRendered() { return isRendered; }
	void ToggleRender() { isRendered != isRendered; }

private:
	Model m_model;
	OrientedBoundingBox modelOBB;
	bool isRendered;
};

#endif