#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "../../pch.h"
#include "../../Engine/Core/Component.h"

class BoxCollider : public Component
{
public:
	// overridden functions
	void OnAttach(GameObject* parent) override;
	void OnDetach() {};
	//void Update(float deltaTime) override;

	// Getters
	inline OrientedBoundingBox* GetBoundingBox() { return obb; }

	inline void SetBoundingBox(OrientedBoundingBox* obb_) { obb = obb_; }

private:
	OrientedBoundingBox* obb;
};
#endif