#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../../pch.h"

struct BoundingBox {
	DirectX::XMFLOAT3 maxVert, minVert;
	DirectX::XMMATRIX transform;

	BoundingBox() {
		maxVert = minVert = DirectX::XMFLOAT3();
		transform = DirectX::XMMATRIX();
	}

	BoundingBox(DirectX::XMFLOAT3 maxVert_, DirectX::XMFLOAT3 minVert_, DirectX::XMMATRIX transform_) {
		maxVert = maxVert_;
		minVert = minVert_;
		transform = transform_;
	}

	bool Intersects(BoundingBox* bbox_)
	{
		// Anytime large amounts of vec/mat math is needed, use SimpleMath extension
		DirectX::SimpleMath::Vector3 miV_ = minVert;
		DirectX::SimpleMath::Vector3 mxV_ = maxVert;
		DirectX::SimpleMath::Vector3 minCorner = GetTransformedPoint(miV_, transform);
		DirectX::SimpleMath::Vector3 maxCorner = GetTransformedPoint(mxV_, transform);

		DirectX::SimpleMath::Vector3 bmiV_ = bbox_->minVert;
		DirectX::SimpleMath::Vector3 bmxV_ = bbox_->maxVert;
		DirectX::SimpleMath::Vector3 _ominCorner = GetTransformedPoint(bmiV_, bbox_->transform);
		DirectX::SimpleMath::Vector3 _omaxCorner = GetTransformedPoint(bmxV_, bbox_->transform);

		return (minCorner.x <= _omaxCorner.x && maxCorner.x >= _ominCorner.x) &&
			(minCorner.y <= _omaxCorner.y && maxCorner.y >= _ominCorner.y) &&
			(minCorner.z <= _omaxCorner.z && maxCorner.z >= _ominCorner.z);
	}

private:
	DirectX::SimpleMath::Vector3 GetTransformedPoint(DirectX::SimpleMath::Vector3 point_, DirectX::SimpleMath::Matrix transform_)
	{
		DirectX::SimpleMath::Vector3 v(transform_._41, transform_._42, transform_._43);
		return (v + point_);
	}
};

#endif