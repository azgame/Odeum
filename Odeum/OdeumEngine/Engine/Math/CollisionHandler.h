#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "../../pch.h"
#include "Ray.h"
#include "../Rendering/GameObject.h"

class CollisionHandler
{
public:
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator=(const CollisionHandler&) = delete;
	CollisionHandler& operator=(CollisionHandler&&) = delete;

	static CollisionHandler* GetInstance();

	void Initialize();
	void AddObject(GameObject* go_);
	void MouseUpdate(DirectX::Mouse::State& mouse);
	void Uninitialize();

private:
	CollisionHandler();
	~CollisionHandler();

	static std::unique_ptr<CollisionHandler>			collisionInstance;
	friend std::default_delete<CollisionHandler>;

	static std::vector<GameObject*> colliders;
	static std::vector<GameObject*> previousCollisions;
};

#endif