#include "OctSpatialPartition.h"
#include <algorithm>

OctNode::OctNode(Vector3 pos_, float sz_, OctNode* parent_)
	: octBounds(nullptr), parent(nullptr), children(), m_objectList(std::vector<GameObject*>())
{
	m_objectList.reserve(10);
	octBounds = new OrientedBoundingBox(pos_, pos_ + Vector3(sz_, sz_, sz_));
	octBounds->center = Vector3(0.0f, 0.0f, 0.0f);

	sz = sz_;

	parent = parent_;

	for (int i = 0; i < CHILD_COUNT; i++)
		children[i] = nullptr;
}

OctNode::~OctNode()
{
	Uninitialize();
}

void OctNode::Uninitialize()
{
	SAFE_DELETE(octBounds);

	if (m_objectList.size() > 0)
	{
		for (auto o : m_objectList)
			o = nullptr;
		m_objectList.clear();
	}
		
	for (int i = 0; i < CHILD_COUNT; i++)
		SAFE_DELETE(children[i]);
}

void OctNode::Octify(int depth_)
{
	if (depth_ <= 0 || this == nullptr) // break statement
		return;

	float half = sz / 2.0f;

	children[static_cast<int>(OctChildren::OCT_TLF)] = new OctNode(
		Vector3(octBounds->GetMin()),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_BLF)] = new OctNode(
		Vector3(octBounds->GetMin()),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_BRF)] = new OctNode(
		Vector3(octBounds->GetMin()),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_TRF)] = new OctNode(
		Vector3(octBounds->GetMin()),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_TLR)] = new OctNode(
		Vector3(octBounds->GetMin().GetX(), octBounds->GetMin().GetY() + half, octBounds->GetMin().GetZ() + half),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_BLR)] = new OctNode(
		Vector3(octBounds->GetMin().GetX(), octBounds->GetMin().GetY(), octBounds->GetMin().GetZ() + half),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_BRR)] = new OctNode(
		Vector3(octBounds->GetMin().GetX() + half, octBounds->GetMin().GetY(), octBounds->GetMin().GetZ() + half),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_TRR)] = new OctNode(
		Vector3(octBounds->GetMin().GetX() + half, octBounds->GetMin().GetY() + half, octBounds->GetMin().GetZ() + half),
		half,
		this
	);

	for (int i = 0; i < CHILD_COUNT; i++)
		children[i]->Octify(depth_ - 1);
}

OctNode* OctNode::getParent()
{
	return parent;
}

OctNode* OctNode::getChild(OctChildren childPos_)
{
	return children[static_cast<int>(childPos_)];
}

void OctNode::addCollisionObject(GameObject* go_)
{
	m_objectList.push_back(go_);
}

size_t OctNode::getObjectCount() const
{
	return m_objectList.size();
}

bool OctNode::isLeaf() const
{
	return children[0] == nullptr;
}

OrientedBoundingBox& OctNode::getBoundingBox()
{
	return *octBounds;
}

OctSpatialPartition::OctSpatialPartition(float worldsz_)
	: root(nullptr), m_rayIntersectionList(std::vector<OctNode*>())
{
	root = new OctNode(Vector3(-(worldsz_ / 2.0f), -(worldsz_ / 2.0f), -(worldsz_ / 2.0f)), worldsz_, nullptr);
	root->Octify(SPATIAL_DEPTH);

	m_rayIntersectionList.reserve(20);
}

OctSpatialPartition::~OctSpatialPartition()
{
	Uninitialize();
}

void OctSpatialPartition::Uninitialize()
{
	if (m_rayIntersectionList.size() > 0)
	{
		for (auto cell : m_rayIntersectionList)
			cell = nullptr;

		m_rayIntersectionList.clear();
	}
	
	SAFE_DELETE(root);
}

void OctSpatialPartition::AddObject(GameObject* go_)
{
	ASSERT(root != nullptr, "Spatial partition has not been initialized and you're trying to add objects to it.");
	AddObjectToCell(root, go_);
}

GameObject* OctSpatialPartition::GetCollision(Ray& ray_, Vector4* IntersectionPlane)
{
	if (m_rayIntersectionList.size() > 0)
	{
		for (auto cell : m_rayIntersectionList)
			cell = nullptr;
		m_rayIntersectionList.clear();
	}
		
	PrepareCollisionQuery(root, ray_);

	GameObject* result = nullptr;
	OctNode* node = nullptr;
	float shortestDistance = FLT_MAX;

	for (auto cell : m_rayIntersectionList)
	{
		for (auto object : cell->m_objectList)
		{
			if (ray_.IsColliding(object->GetBoundingBox(), IntersectionPlane))
			{
				if (ray_.t < shortestDistance)
				{
					node = cell;
					result = object;
					shortestDistance = ray_.t;
				}
			}	
		}

		if (result != nullptr)
		{
			// CollisionDetection::RayOBBIntersectionPlane(ray_, result->GetBoundingBox(), IntersectionPlane);
			return result;
		}
	}

	return nullptr;
}

std::vector<GameObject*> OctSpatialPartition::GetCollisions(Ray& ray)
{
	if (m_rayIntersectionList.size() > 0)
	{
		for (auto cell : m_rayIntersectionList)
			cell = nullptr;
		m_rayIntersectionList.clear();
	}

	PrepareCollisionQuery(root, ray);

	GameObject* result = nullptr;
	OctNode* node = nullptr;
	float shortestDistance = FLT_MAX;

	std::vector<GameObject*> collisions;

	for (auto cell : m_rayIntersectionList)
	{
		for (auto object : cell->m_objectList)
		{
			if (ray.IsColliding(object->GetBoundingBox(), nullptr))
			{
				if (ray.t < shortestDistance)
				{
					node = cell;
					result = object;
					shortestDistance = ray.t;
				}

				collisions.push_back(object);
			}
		}
	}

	return collisions;
}

void OctSpatialPartition::UpdatePartition()
{
	UpdatePartitionCell(root);
}

void OctSpatialPartition::AddObjectToCell(OctNode* cell_, GameObject* go_)
{
	if (cell_->isLeaf())
	{
		if (go_->GetBoundingBox().Intersects(cell_->getBoundingBox()))
			cell_->addCollisionObject(go_);
		return;
	}

	for (auto cell : cell_->children)
	{
		if (cell->isLeaf() && go_->GetBoundingBox().Intersects(cell->getBoundingBox()))
		{
			cell->addCollisionObject(go_);
			return;
		}

		if (go_->GetBoundingBox().Intersects(cell_->getBoundingBox()))
			AddObjectToCell(cell, go_);
	}
}

void OctSpatialPartition::PrepareCollisionQuery(OctNode* cell_, Ray ray_)
{
	if (cell_->isLeaf())
	{
		if (ray_.IsColliding(cell_->getBoundingBox(), nullptr))
			m_rayIntersectionList.push_back(cell_);
		return;
	}

	for (auto cell : cell_->children)
	{
		if (cell->isLeaf() && ray_.IsColliding(cell->getBoundingBox(), nullptr))
		{
			m_rayIntersectionList.push_back(cell);
			return;
		}

		if (ray_.IsColliding(cell->getBoundingBox(), nullptr))
			PrepareCollisionQuery(cell, ray_);
	}
}

void OctSpatialPartition::UpdatePartitionCell(OctNode* cell_)
{
	if (cell_->isLeaf())
	{
		for (auto go : cell_->m_objectList)
		{
			if (!go->GetBoundingBox().Intersects(cell_->getBoundingBox()))
			{
				std::remove(cell_->m_objectList.begin(), cell_->m_objectList.end(), go);
				AddObject(go);
			}

			// Check if object collides with other objects in list using simple colliders
			// if they do, check to see if either have complex colliders
			// if they do, do gjk
		}

		return;
	}

	for (auto cell : cell_->children)
		UpdatePartitionCell(cell);
}

void OctSpatialPartition::RemoveObjectFromCell(OctNode* cell_, GameObject* go_)
{
}
