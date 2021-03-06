#ifndef OCTSPATIALPARTITION_H
#define OCTSPATIALPARTITION_H

#include "../../pch.h"
#include "Ray.h"
#include "../Core/GameObject.h"
#include "../../Game/Components/BoxCollider.h"

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
	OctNode(Vector3 pos_, float sz_, OctNode* parent_);
	~OctNode();
	void Uninitialize();
	void Octify(int depth_);
	OctNode* getParent();
	OctNode* getChild(OctChildren childPos_);
	void addCollisionObject(BoxCollider* go_);
	size_t getObjectCount() const;
	bool isLeaf() const;
	OrientedBoundingBox& getBoundingBox();

private:
	friend class OctSpatialPartition;
	OrientedBoundingBox* octBounds;
	OctNode* parent;
	OctNode* children[CHILD_COUNT];

	std::vector<BoxCollider*> m_objectList;
	float sz;
};

class OctSpatialPartition
{
public:
	OctSpatialPartition(float worldsz_);
	~OctSpatialPartition();

	void Uninitialize();

	void AddObject(BoxCollider* go_);
	BoxCollider* GetCollision(Ray& ray_, Vector4* IntersectionPlane);
	std::vector<BoxCollider*> GetCollisions(Ray& ray);
	void UpdatePartition();

private:

	OctNode* root;
	std::vector<OctNode*> m_rayIntersectionList;
	void AddObjectToCell(OctNode* cell_, BoxCollider* go_);
	void PrepareCollisionQuery(OctNode* cell_, Ray ray_);
	void UpdatePartitionCell(OctNode* cell_);
	void RemoveObjectFromCell(OctNode* cell_, BoxCollider* go_);
};

#endif