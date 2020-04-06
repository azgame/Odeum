#include "OctSpatialPartition.h"

OctNode::OctNode(DirectX::SimpleMath::Vector3 pos_, float sz_, OctNode* parent_)
	: octBounds(nullptr), parent(nullptr), children(), m_objectList(std::vector<GameObject*>())
{
	m_objectList.reserve(10);
	octBounds = new BoundingBox();
	octBounds->minVert = pos_;
	octBounds->maxVert = pos_ + DirectX::SimpleMath::Vector3(sz_, sz_, sz_);

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
		DirectX::SimpleMath::Vector3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_BLF)] = new OctNode(
		DirectX::SimpleMath::Vector3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_BRF)] = new OctNode(
		DirectX::SimpleMath::Vector3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_TRF)] = new OctNode(
		DirectX::SimpleMath::Vector3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_TLR)] = new OctNode(
		DirectX::SimpleMath::Vector3(octBounds->minVert.x, octBounds->minVert.y + half, octBounds->minVert.z + half),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_BLR)] = new OctNode(
		DirectX::SimpleMath::Vector3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z + half),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_BRR)] = new OctNode(
		DirectX::SimpleMath::Vector3(octBounds->minVert.x + half, octBounds->minVert.y, octBounds->minVert.z + half),
		half,
		this
	);

	children[static_cast<int>(OctChildren::OCT_TRR)] = new OctNode(
		DirectX::SimpleMath::Vector3(octBounds->minVert.x + half, octBounds->minVert.y + half, octBounds->minVert.z + half),
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

int OctNode::getObjectCount() const
{
	return m_objectList.size();
}

bool OctNode::isLeaf() const
{
	return children[0] == nullptr;
}

BoundingBox* OctNode::getBoundingBox()
{
	return octBounds;
}

OctSpatialPartition::OctSpatialPartition(float worldsz_)
	: root(nullptr), m_rayIntersectionList(std::vector<OctNode*>())
{
	root = new OctNode(DirectX::SimpleMath::Vector3(-(worldsz_ / 2.0f), -(worldsz_ / 2.0f), -(worldsz_ / 2.0f)), worldsz_, nullptr);
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
	AddObjectToCell(root, go_);
}

GameObject* OctSpatialPartition::GetCollision(Ray ray_)
{
	if (m_rayIntersectionList.size() > 0)
	{
		for (auto cell : m_rayIntersectionList)
			cell = nullptr;
		m_rayIntersectionList.clear();
	}
		
	PrepareCollisionQuery(root, ray_);

	GameObject* result = nullptr;
	float shortestDistance = FLT_MAX;

	for (auto cell : m_rayIntersectionList)
	{
		for (auto object : cell->m_objectList)
		{
			if (ray_.isColliding(&object->GetBoundingBox()))
			{
				if (ray_.t < shortestDistance)
				{
					result = object;
					shortestDistance = ray_.t;
				}
			}	
		}

		if (result != nullptr)
			return result;
	}

	return nullptr;
}

void OctSpatialPartition::AddObjectToCell(OctNode* cell_, GameObject* go_)
{
	// TODO - Aidan: Enter cell that object aabb intersects with, and if cell is a leaf node add object to cell, if not recursively call this on children


}

void OctSpatialPartition::PrepareCollisionQuery(OctNode* cell_, Ray ray_)
{
	// TODO - Aidan: Enter cell that ray colliders with, is this cell a leaf node add it to rayintersectionlist, if not recursively call this on children


}