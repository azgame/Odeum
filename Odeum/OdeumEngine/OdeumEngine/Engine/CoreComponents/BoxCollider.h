#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "../../pch.h"
#include "ColliderComponent.h"

class BoxCollider : public ColliderComponent
{
public:
	void OnAttach(GameObject* parent) override;
	void OnDetach() override {};
	void OnStart() {};
	void Update(float deltaTime) override;

	// Getters
	inline OrientedBoundingBox* GetBoundingBox() { return obb; }
	inline void SetBoundingBox(OrientedBoundingBox* obb_) { obb = obb_; }

private:
	OrientedBoundingBox* obb;
};

#endif