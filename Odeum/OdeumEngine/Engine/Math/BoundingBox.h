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
};

#endif // !BOUNDINGBOX_H
