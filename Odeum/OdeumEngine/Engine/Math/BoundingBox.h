#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../../pch.h"

struct BoundingBox {
	DirectX::XMFLOAT3 maxVert, minVert;
	DirectX::XMMATRIX transform;

	inline BoundingBox() {
		maxVert = minVert = DirectX::XMFLOAT3();
		transform = DirectX::XMMATRIX();
	}

	inline BoundingBox(DirectX::XMFLOAT3 maxVert_, DirectX::XMFLOAT3 minVert_, DirectX::XMMATRIX transform_) {
		maxVert = maxVert_;
		minVert = minVert_;
		transform = transform_;
	}

	inline bool Intersects(BoundingBox* bbox_)
	{
		// Anytime large amounts of vec/mat math is needed, use SimpleMath extension
		DirectX::SimpleMath::Vector3 minCorner = GetTransformedPoint(minVert, transform);
		DirectX::SimpleMath::Vector3 maxCorner = GetTransformedPoint(maxVert, transform);

		DirectX::SimpleMath::Vector3 _ominCorner = GetTransformedPoint(bbox_->minVert, bbox_->transform);
		DirectX::SimpleMath::Vector3 _omaxCorner = GetTransformedPoint(bbox_->maxVert, bbox_->transform);

		return (minCorner.x <= _omaxCorner.x && maxCorner.x >= _ominCorner.x) &&
			(minCorner.y <= _omaxCorner.y && maxCorner.y >= _ominCorner.y) &&
			(minCorner.z <= _omaxCorner.z && maxCorner.z >= _ominCorner.z);
	}

private:
	inline DirectX::SimpleMath::Vector3 GetTransformedPoint(DirectX::SimpleMath::Vector3 point_, DirectX::SimpleMath::Matrix transform_)
	{
		return DirectX::SimpleMath::Vector3(transform_._41, transform_._42, transform_._43) + point_;
	}
};

#endif // !BOUNDINGBOX_H
