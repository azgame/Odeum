#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "../../pch.h"

#include "../Core/GameObject.h"
#include "../Core/Input.h"

#include "Ray.h"
#include "OctSpatialPartition.h"

class CollisionHandler
{
public:
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator=(const CollisionHandler&) = delete;
	CollisionHandler& operator=(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

	void Initialize(float worldsz_);
	void AddObject(GameObject* go_);
	bool MouseCollide();
	void RayQueryFirst(Ray& ray, Vector4** IntersectionPlane);
	void RayQueryList(Ray& ray, std::vector<GameObject*>& IntersectedObjects);
	GameObject* RayGetFirstHit(Ray& ray, Vector4** IntersectionPlane = nullptr);
	std::vector<GameObject*>& RayGetList(Ray& ray);
	void Update();
	void Uninitialize();

private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler>			collisionInstance;
	friend std::default_delete<CollisionHandler>;

	static std::vector<GameObject*>						previousCollisions;

	static OctSpatialPartition*							m_scenePartition;
};

#endif