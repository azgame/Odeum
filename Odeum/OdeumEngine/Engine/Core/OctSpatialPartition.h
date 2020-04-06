#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include "../../pch.h"
#include "../Math/Ray.h"
#include "../Rendering/GameObject.h"

#define CHILD_COUNT 8
#define SPATIAL_DEPTH 3

enum class OctChildren
{
	OCT_TLF,
	OCT_BLF,
	OCT_BRF,
	OCT_TRF,
	OCT_TLR,
	OCT_BLR,
	OCT_BRR,
	OCT_TRR
};

struct OctNode
{
	OctNode(DirectX::SimpleMath::Vector3 pos_, float sz_, OctNode* parent_);
	~OctNode();
	void Uninitialize();
	void Octify(int depth_);
	OctNode* getParent();
	OctNode* getChild(OctChildren childPos_);
	void addCollisionObject(GameObject* go_);
	int getObjectCount() const;
	bool isLeaf() const;
	BoundingBox* getBoundingBox();

private:
	friend class OctSpatialPartition;
	BoundingBox* octBounds;
	OctNode* parent;
	OctNode* children[CHILD_COUNT];

	std::vector<GameObject*> m_objectList;
	float sz;
};

class OctSpatialPartition
{
public:
	OctSpatialPartition(float worldsz_);
	~OctSpatialPartition();

	void Uninitialize();

	void AddObject(GameObject* go_);
	GameObject* GetCollision(Ray ray_);
private:

	OctNode* root;
	std::vector<OctNode*> m_rayIntersectionList;
	void AddObjectToCell(OctNode* cell_, GameObject* go_);
	void PrepareCollisionQuery(OctNode* cell_, Ray ray_);

};

#endif