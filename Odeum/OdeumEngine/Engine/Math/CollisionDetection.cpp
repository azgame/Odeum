#include "CollisionDetection.h"

#include "Ray.h"
#include "BoundingBox.h"
#include "MathHelper.h"

#include "../Core/OdeumEngine.h"

CollisionDetection::~CollisionDetection()
{
}

Ray CollisionDetection::ScreenPosToWorldRay(DirectX::XMFLOAT2 mouseCoords_, DirectX::XMFLOAT2 screenSize_, Camera* camera_)
{
	DirectX::XMFLOAT4 raystart_NDC((mouseCoords_.x / screenSize_.x - 0.5f) * 2.0f,
		(mouseCoords_.y / screenSize_.y - 0.5f) * 2.0f,
		-1.0f, 1.0f);

	DirectX::XMFLOAT4 rayend_NDC((mouseCoords_.x / screenSize_.x - 0.5f) * 2.0f,
		(mouseCoords_.y / screenSize_.y - 0.5f) * 2.0f,
		0.0f, 1.0f);

	DirectX::XMMATRIX inverse = DirectX::XMMatrixInverse(nullptr, camera_->View() * camera_->Projection());

	DirectX::XMFLOAT4 raystart_World;
	DirectX::XMStoreFloat4(&raystart_World, DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&raystart_NDC), inverse));
	raystart_World.x /= raystart_World.w;
	raystart_World.y /= raystart_World.w;
	raystart_World.z /= raystart_World.w;
	raystart_World.w /= raystart_World.w;

	DirectX::XMFLOAT4 rayend_World;
	DirectX::XMStoreFloat4(&rayend_World, DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&rayend_NDC), inverse));
	rayend_World.x /= rayend_World.w;
	rayend_World.y /= rayend_World.w;
	rayend_World.z /= rayend_World.w;
	rayend_World.w /= rayend_World.w;

	DirectX::XMFLOAT4 raydir_World;
	DirectX::XMStoreFloat4(&raydir_World, DirectX::XMVector4Normalize(DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(&rayend_World), DirectX::XMLoadFloat4(&raystart_World))));

	return Ray(Float4To3(raystart_World), Float4To3(raydir_World));
}

bool CollisionDetection::RayOBBIntersection(Ray* ray_, BoundingBox* box_)
{
	float tMin = OdeumEngine::GetInstance()->GetCamera()->NearClipPlane();
	float tMax = OdeumEngine::GetInstance()->GetCamera()->FarClipPlane();

	// Have to use the dxtk for this since built in float.../vector conversion would get hefty (see above)
	DirectX::SimpleMath::Vector3 obbPos(box_->transform.r[3]);
	DirectX::SimpleMath::Vector3 delta = obbPos - ray_->st;

	DirectX::SimpleMath::Vector3 xAxis(box_->transform.r[0]);

	float e = delta.Dot(xAxis);
	float f = xAxis.Dot(ray_->dir);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + box_->minVert.x) / f;
		float t2 = (e + box_->maxVert.x) / f;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax)		
			tMax = t2;
		if (t1 > tMin)		
			tMin = t1;

		if (tMax < tMin)	
			return false;
	}
	else
		if (-e + box_->minVert.x > 0.0f || -e + box_->maxVert.x < 0.0f)
			return false;

	DirectX::SimpleMath::Vector3 yAxis(box_->transform.r[1]);

	e = delta.Dot(yAxis);
	f = yAxis.Dot(ray_->dir);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + box_->minVert.y) / f;
		float t2 = (e + box_->maxVert.y) / f;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax)
			tMax = t2;
		if (t1 > tMin)
			tMin = t1;

		if (tMax < tMin)
			return false;
	}
	else
		if (-e + box_->minVert.y > 0.0f || -e + box_->maxVert.y < 0.0f)
			return false;

	DirectX::SimpleMath::Vector3 zAxis(box_->transform.r[2]);

	e = delta.Dot(zAxis);
	f = zAxis.Dot(ray_->dir);

	if (fabs(f) > 0.001f)
	{
		float t1 = (e + box_->minVert.z) / f;
		float t2 = (e + box_->maxVert.z) / f;

		if (t1 > t2)
		{
			float w = t1;
			t1 = t2;
			t2 = w;
		}

		if (t2 < tMax)
			tMax = t2;
		if (t1 > tMin)
			tMin = t1;

		if (tMax < tMin)
			return false;
	}
	else
		if (-e + box_->minVert.z > 0.0f || -e + box_->maxVert.z < 0.0f)
			return false;

	ray_->t = tMin;

	return true;
}
