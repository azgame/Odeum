#include "NavMeshManager.h"

#include "../Math/Collider.h"
#include "../Math/Plane.h"

#include <bitset>

// This seems useful, I'll move it out of here later
struct BitArray
{
	BitArray(size_t Size)
	{
		cellSize = 32;
		m_bitMap.reserve(Size / cellSize);
	}

	void Reset(size_t Size)
	{
		m_bitMap.clear();
		m_bitMap.reserve(Size / cellSize);
	}

	void Set(uint32_t Pos, bool Val = true)
	{
		ASSERT(Pos < m_bitMap.size() * cellSize, "Value exceeds bounds of bit array");
		uint32_t ArrayCell = Pos / (uint32_t)cellSize;
		uint32_t Index = Pos - (ArrayCell * (uint32_t)cellSize);
		if (Val) m_bitMap[ArrayCell] |= (1UL << Index);
		else m_bitMap[ArrayCell] &= ~(1UL << Index);
	}

	bool Check(uint32_t Pos)
	{
		ASSERT(Pos < m_bitMap.size()* cellSize, "Trying to check bit array value outside bounds of array");
		uint32_t ArrayCell = Pos / (uint32_t)cellSize;
		uint32_t Index = Pos - (ArrayCell * (uint32_t)cellSize);
		return m_bitMap[ArrayCell] & (1UL << Index);
	}

	std::vector<uint32_t> m_bitMap;
	size_t cellSize;
};

struct BitGrid
{
	BitGrid(size_t Width, size_t Height) : m_bitmap(Width * Height)
	{
		width = Width;
		height = Height;
	}

	void Reset(size_t Width, size_t Height)
	{
		m_bitmap.Reset(Width * Height);
	}

	void Set(uint32_t X, uint32_t Y)
	{
		ASSERT(0 < X && X < width && 0 < Y && Y < height, "Coordinates given exceed bounds of Bit Grid!");
		m_bitmap.Set(Y * (uint32_t)width + X);
	}

	void Set(Vector2 Coordinate)
	{
		ASSERT(0 < Coordinate.GetX() && Coordinate.GetX() < width && 0 < Coordinate.GetY() && Coordinate.GetY() < height, "Coordinates given exceed bounds of Bit Grid!");
		m_bitmap.Set(Coordinate.GetY() * (uint32_t)width + Coordinate.GetX());
	}

	size_t width, height;
	BitArray m_bitmap;
};

namespace NavMeshManager
{
	BitGrid m_binaryMap(20, 20);

	std::vector<Vector2> m_contourPoints;
}


// Not even sure what we need to initialize yet, maybe a list of nav meshes? loading baked nav meshes from file
void NavMeshManager::Initialize()
{

}

// Might not use Colliders in interface yet, going to start by grabbing obbs from scene and translating them into colliders (slow) and speed it up later
// A necessary optimization will be to spatially partition/order obstacles (preferably using BVH)
void NavMeshManager::GenerateNavMesh(uint32_t CellSize, Plane GroundPlane, std::vector<Collider2D> Obstacles)
{
	// For now...
	ASSERT(GroundPlane.normal == Vector3(kYUnitVector), "For now, we are only accepting planes which lie 'flat' and face 'up'");

	// Get plane size (GroundPlane min/max divided by CellSize)
	uint32_t totalWidth, totalHeight;
	totalWidth = (uint32_t)(GroundPlane.max.GetX() - GroundPlane.min.GetX());
	totalHeight = (uint32_t)(GroundPlane.max.GetZ() - GroundPlane.min.GetZ());

	// Determine max extent via CellSize
	uint32_t numColumns, numRows;
	numColumns = totalWidth / CellSize;
	numRows = totalHeight / CellSize;

	// Start at 0, 0 in plane local space. Step +x CellSize. Create collider2D starting at Step position (min) and extent of cell (CellSize + x, CellSize + y)
	
	
	// Check collision of collider 2D against Obstacles, setting bitgrid at index if collision 
	// While doing this, following up on previous cell with contouring if past 1st row
	// Check marching square and give cell value (0-15) for 2x2 cells. Create contour points at boundaries of binary image
	
}

void NavMeshManager::Uninitialize()
{

}