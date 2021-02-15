#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "../../pch.h"

#include "../Core/GameObject.h"
#include "../Core/Input.h"

#include "Ray.h"
#include "OctSpatialPartition.h"
#include "../Game/Components/SphereCollider.h"

#include "../Game/Components/ComplexCollider.h"
#include "CollisionPoints.h"

class CollisionHandler
{
public:
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator=(const CollisionHandler&) = delete;
	CollisionHandler& operator=(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

	void Initialize(float worldsz_);
	void AddObject(BoxCollider* go_);
	bool MouseCollide();
	Ray GetMouseRay();
	void RayQueryFirst(Ray& ray, Vector4* IntersectionPlane);
	void RayQueryList(Ray& ray, std::vector<BoxCollider*>& IntersectedObjects);
	BoxCollider* RayGetFirstHit(Ray& ray, Vector4* IntersectionPlane = nullptr);
	std::vector<BoxCollider*>& RayGetList(Ray& ray);
	void Update();
	void Uninitialize();

	void SphereSphereCollisionResponse(SphereCollider& sc1, SphereCollider& sc2, float e);
	void SphereStaticBoxCollisionResponse(SphereCollider& sc, BoxCollider& bc);
	void OBBOBBCollisionRespones(BoxCollider& bc1, BoxCollider& bc2);
	void GJKCollisionResponse(ComplexCollider* cc1, ComplexCollider* cc2, Simplex<Vector3>& simplex);

private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler>			collisionInstance;
	friend std::default_delete<CollisionHandler>;

	static std::vector<BoxCollider*>						previousCollisions;

	static OctSpatialPartition*							m_scenePartition;
};

#endif