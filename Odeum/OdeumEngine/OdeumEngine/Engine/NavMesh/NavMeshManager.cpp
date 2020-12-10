#include "NavMeshManager.h"

#include "../Math/Collider.h"
#include "../Math/Plane.h"

#include "../Math/CollisionDetection.h"

#include <unordered_map>
#include <list>
#include <bitset>

// This seems useful, I'll move it out of here later
struct BitArray
{
	BitArray(size_t Size)
	{
		cellSize = 32;
		m_bitMap.reserve(Size / cellSize);
		std::fill(m_bitMap.begin(), m_bitMap.end(), 0);
	}

	void Reset(size_t Size)
	{
		m_bitMap.clear();
		m_bitMap.resize(Size / cellSize);
		std::fill(m_bitMap.begin(), m_bitMap.end(), 0);
	}

	void Set(uint32_t Pos, bool Val = true)
	{
		ASSERT(Pos < m_bitMap.size() * cellSize, "Value exceeds bounds of bit array");
		uint32_t ArrayCell = Pos / (uint32_t)cellSize;
		uint32_t Index = Pos - (ArrayCell * (uint32_t)cellSize);
		if (Val) m_bitMap[ArrayCell] |= 1UL << Index;
		else m_bitMap[ArrayCell] &= 1UL << ~Index;
	}

	bool Check(uint32_t Pos)
	{
		ASSERT(Pos < m_bitMap.size() * cellSize, "Trying to check bit array value outside bounds of array");
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
		width = Width;
		height = Height;
	}

	void Set(uint32_t X, uint32_t Y, bool Val = true)
	{
		ASSERT(0 <= X && X < width && 0 <= Y && Y < height, "Coordinates given exceed bounds of Bit Grid!");
		m_bitmap.Set(Y * (uint32_t)width + X, Val);
	}

	void Set(Vector2 Coordinate, bool Val = true)
	{
		ASSERT(0 <= Coordinate.GetX() && Coordinate.GetX() < width && 0 <= Coordinate.GetY() && Coordinate.GetY() < height, "Coordinates given exceed bounds of Bit Grid!");
		m_bitmap.Set(Coordinate.GetY() * (uint32_t)width + Coordinate.GetX(), Val);
	}

	bool Check(uint32_t X, uint32_t Y)
	{
		ASSERT(0 <= X && X < width && 0 <= Y && Y < height, "Coordinates given exceed bounds of Bit Grid!");
		return m_bitmap.Check(Y * (uint32_t)width + X);
	}

	size_t width, height;
	BitArray m_bitmap;
};

/* Implementation */
namespace NavMeshManager
{
	BitGrid m_binaryGrid(20, 20);
	std::vector<uint16_t> m_marchingCells;
	std::vector<std::vector<Vector2>> m_contourPoints;

	std::unordered_map<uint16_t, Vector4> lookupMarchingSquares;

	void GenerateBinaryImage(float CellSize, uint32_t NumColumns, uint32_t NumRows, Plane GroundPlane, std::vector<Collider2D> Obstacles);
	void CreateContourPoints(uint32_t XIndex, uint32_t ZIndex);
	void CreateContourPoints();
	std::vector<Vector2> ReduceContourPointsRDP(std::vector<Vector2> PointList, float Epsilon);
}


// For now just initializing the lookup table for finding the edge lines of a 2x2 cell generated by marching squares
void NavMeshManager::Initialize()
{
	lookupMarchingSquares.reserve(16);
	lookupMarchingSquares[0] = Vector4(kZero);

	lookupMarchingSquares[1] = Vector4(0.5f, 1.0f, 1.0f, 1.5f);
	lookupMarchingSquares[2] = Vector4(1.0f, 1.5f, 1.5f, 1.0f);
	lookupMarchingSquares[3] = Vector4(0.5f, 1.0f, 1.5f, 1.0f);
	lookupMarchingSquares[4] = Vector4(1.5f, 1.0f, 1.0f, 0.5f);

	lookupMarchingSquares[5] = Vector4(kZero); // saddle point, ignore for now

	lookupMarchingSquares[6] = Vector4(1.0f, 1.5f, 1.0f, 0.5f);
	lookupMarchingSquares[7] = Vector4(0.5f, 1.0f, 1.0f, 0.5f);
	lookupMarchingSquares[8] = Vector4(1.0f, 0.5f, 0.5f, 1.0f);
	lookupMarchingSquares[9] = Vector4(1.0f, 0.5f, 1.0f, 1.5f);

	lookupMarchingSquares[10] = Vector4(kZero); // saddle point, ignore for now

	lookupMarchingSquares[11] = Vector4(1.0f, 0.5f, 1.5f, 1.0f);
	lookupMarchingSquares[12] = Vector4(1.5f, 1.0f, 0.5f, 1.0f);
	lookupMarchingSquares[13] = Vector4(1.5f, 1.0f, 1.0f, 1.5f); //
	lookupMarchingSquares[14] = Vector4(1.0f, 1.5f, 0.5f, 1.0f);

	lookupMarchingSquares[15] = Vector4(kZero);
}

// Might not use Colliders in interface yet, going to start by grabbing obbs from scene and translating them into colliders (slow) and speed it up later
// A necessary optimization will be to spatially partition/order obstacles (preferably using BVH)
void NavMeshManager::GenerateNavMesh(float CellSize, Plane GroundPlane, std::vector<Collider2D> Obstacles)
{
	// For now...
	ASSERT(GroundPlane.normal == Vector3(kYUnitVector), "For now, we are only accepting planes which lie 'flat' and face 'up'");

	// Get plane size (GroundPlane min/max divided by CellSize)
	uint32_t totalWidth, totalHeight;
	totalWidth = (uint32_t)(GroundPlane.max.GetX() - GroundPlane.min.GetX());
	totalHeight = (uint32_t)(GroundPlane.max.GetZ() - GroundPlane.min.GetZ());

	float startX, startZ;
	startX = GroundPlane.min.GetX();
	startZ = GroundPlane.min.GetZ();

	// Determine max extent via CellSize
	uint32_t numColumns, numRows;
	numColumns = (float)totalWidth / CellSize;
	numRows = (float)totalHeight / CellSize;

	m_binaryGrid.Reset(numColumns, numRows);
	m_marchingCells.reserve((numRows - 1) * (numColumns - 1));

	Collider2D* cachedCollider = nullptr;

	// Start at 0, 0 in plane local space. Step +x CellSize. Create collider2D starting at Step position (min) and extent of cell (CellSize + x, CellSize + y)
	// TODO Aidan: Parallelize this later
	for (UINT j = 0; j < numRows; j++)
	{
		for (UINT i = 0; i < numColumns; i++)
		{
			Collider2D cell = 
			{ 
				/*BL*/ Vector2(i * CellSize + startX, j * CellSize + startZ), 
				/*BR*/ Vector2(i * CellSize + CellSize + startX, j * CellSize + startZ),
				/*TL*/ Vector2(i * CellSize + startX, j * CellSize + CellSize + startZ),
				/*TR*/ Vector2(i * CellSize + CellSize + startX, j * CellSize + CellSize + startZ)
			};

			// If in the last loop we collided, we cache the collider we hit for faster lookup in the next frame 
			// since its likely that it will be a part of a larger contiguous shape
			/*if (cachedCollider != nullptr)
			{
				if (!CollisionDetection::GJKCollisionDetection2D(&cell, cachedCollider))
				{
					m_binaryGrid.Set(i, j);
					std::cout << "X";
					continue;
				}
			}*/

			bool collision = false;

			// Check collision of collider 2D against Obstacles, setting bitgrid at index if collision 
			for (auto c : Obstacles)
			{
				//if (cachedCollider == &c) // skip the collider that we've already checked
				//	continue;

				if (CollisionDetection::GJKCollisionDetection2D(&cell, &c))
				{
					cachedCollider = &c;
					m_binaryGrid.Set(i, j);
					collision = true;
					break;
				}
			}

			if (!collision)
			{
				cachedCollider = nullptr;
			}	
		}
	}

	for (int j = numRows - 1; j >= 0; j--)
	{
		for (int i = 0; i < numColumns; i++)
		{
			if (m_binaryGrid.Check(i, j))
			{
				std::cout << "X";
			}
			else
			{
				std::cout << "O";
			}
		}
		std::cout << "" << std::endl;
	}

	CreateContourPoints();

	auto halfway = m_contourPoints[0].begin() + (m_contourPoints[0].size() / 2);

	std::vector<Vector2> firstHalf = ReduceContourPointsRDP(std::vector<Vector2>(m_contourPoints[0].begin(), halfway), 1.5f);
	std::vector<Vector2> secondHalf = ReduceContourPointsRDP(std::vector<Vector2>(halfway, m_contourPoints[0].end() - 1), 1.5f);

	cachedCollider = nullptr;
}

// For later
void NavMeshManager::GenerateBinaryImage(float CellSize, uint32_t NumColumns, uint32_t NumRows, Plane GroundPlane, std::vector<Collider2D> Obstacles)
{
}

/*
	March completed binary image. March along contours given direction from start->end of contour line. Once we reach original start, close contour line and save collection to d.s.
*/
// This is our "once completed" contouring. Might try to parallelize this later
void NavMeshManager::CreateContourPoints()
{
	// iterate through grid in 2x2s. If we find a new contour, begin a new list with it as start and end. "Walk" the contour line by getting the next 2x2, 
	//	informed by the direction of end->start

	// Copy binary grid so we can safely overwrite values while creating contouring lines
	BitGrid binaryImage = m_binaryGrid;

	uint32_t width, height;
	width = binaryImage.width;
	height = binaryImage.height;

	for (uint32_t j = 1; j < height; j++)
	{
		for (uint32_t i = 1; i < width; i++)
		{
			uint32_t X, Z;
			X = i - 1;
			Z = j - 1;

			uint16_t bit = 0;
			bit |= binaryImage.Check(X++, Z) ? 0 : 1UL << 3; // msb
			bit |= binaryImage.Check(X, Z++) ? 0 : 1UL << 2;
			bit |= binaryImage.Check(X--, Z) ? 0 : 1UL << 1;
			bit |= binaryImage.Check(X, Z) ? 0 : 1UL; // lsb

			Vector4 bitLine = lookupMarchingSquares[bit];

			if (bitLine == Vector4(kZero))
				continue;

			// Start moving along the contour
			Vector2 start = Vector2(bitLine.GetX() + i - 1, bitLine.GetY() + j - 1);
			Vector2 end = Vector2(bitLine.GetZ() + i - 1, bitLine.GetW() + j - 1);

			m_contourPoints.push_back({ start, end });
			size_t objectIndex = m_contourPoints.size() - 1;

			std::unordered_map<uint32_t, std::pair<uint32_t, uint32_t>> outerWalls;
			outerWalls[j] = { i, i };

			uint32_t xPos, zPos;
			xPos = i;
			zPos = j;

			if (end.GetX() == xPos)
			{
				zPos += end.GetY() > start.GetY() ? 1 : (end.GetY() < start.GetY() ? -1 : 0);
			}
			else if (end.GetY() == zPos)
			{
				xPos += end.GetX() > start.GetX() ? 1 : (end.GetX() < start.GetX() ? -1 : 0);
			}

			// Move along the contour until we reach the beginning
			while (xPos != i || zPos != j)
			{
				// Construct a new 2x2 around end point
				X = xPos - 1;
				Z = zPos - 1;

				ASSERT(0 <= X && X < width - 1 && 0 <= Z && Z < height - 1, "Went outside bounds of grid");

				// We are building the min and max x points for each row so that when we go to clear the binary image, we know
				//	exactly how much to delete
				auto it = outerWalls.find(zPos);
				if (it != outerWalls.end()) 
				{
					// stuff
					if (xPos < it->second.first)
						it->second.first = xPos;
					if (xPos > it->second.second)
						it->second.second = xPos;
				}
				else
				{
					outerWalls[zPos] = { xPos, xPos };
				}

				bit = 0;
				bit |= binaryImage.Check(X++, Z) ? 0 : 1UL << 3; // msb
				bit |= binaryImage.Check(X, Z++) ? 0 : 1UL << 2;
				bit |= binaryImage.Check(X--, Z) ? 0 : 1UL << 1;
				bit |= binaryImage.Check(X, Z) ? 0 : 1UL; // lsb

				bitLine = lookupMarchingSquares[bit];

				if (bitLine == Vector4(kZero))
				{
					ERROR("This shouldn't happen");
				}
					

				start = Vector2(bitLine.GetX() + xPos - 1, bitLine.GetY() + zPos - 1);
				end = Vector2(bitLine.GetZ() + xPos - 1, bitLine.GetW() + zPos - 1);

				m_contourPoints[objectIndex].push_back(end);

				if (end.GetX() == xPos)
				{
					zPos += end.GetY() > start.GetY() ? 1 : (end.GetY() < start.GetY() ? -1 : 0);
				}
				else if (end.GetY() == zPos)
				{
					xPos += end.GetX() > start.GetX() ? 1 : (end.GetX() < start.GetX() ? -1 : 0);
				}
			}

			// erase the object from the temp binary image
			for (auto row : outerWalls)
			{
				for (int i = row.second.first; i < row.second.second; i++)
				{
					binaryImage.Set(i, row.first, false);
				}
			}
		}
	}
}



std::vector<Vector2> NavMeshManager::ReduceContourPointsRDP(std::vector<Vector2> PointList, float Epsilon)
{
	// Created from Ramer-Douglas-Peucker pseudocode found at https://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93Peucker_algorithm

	float dmax = 0;
	uint32_t index = 0;
	size_t end = PointList.size() - 1;

	for (int i = 1; i < end; i++)
	{
		double d = Math::PerpendicularDistance(PointList[i], PointList[0], PointList[end]);
		if (d > dmax)
		{
			index = i;
			dmax = d;
		}
	}

	std::vector<Vector2> finalList;

	if (dmax > Epsilon)
	{
		std::vector<Vector2> results1 = ReduceContourPointsRDP(std::vector<Vector2>(PointList.begin(), PointList.begin() + index), Epsilon);
		std::vector<Vector2> results2 = ReduceContourPointsRDP(std::vector<Vector2>(PointList.begin() + index, PointList.end()), Epsilon);

		finalList.reserve(results1.size() + results2.size());
		finalList.insert(finalList.end(), results1.begin(), results1.end());
		finalList.insert(finalList.end(), results2.begin(), results2.end());
	}
	else
	{
		finalList = std::vector<Vector2>({ PointList[0], PointList[end] });
	}

	return finalList;
}


void NavMeshManager::Uninitialize()
{
}

void NavMeshManager::CreateContourPoints(uint32_t XIndex, uint32_t ZIndex)
{
	//ASSERT(XIndex > 0 && XIndex < m_binaryGrid.width && ZIndex > 0 && ZIndex < m_binaryGrid.height, "Trying to march outside the bounds of the grid, thats a paddlin");

	//uint16_t bit = 0;

	//uint32_t X, Z;
	//X = XIndex - 1;
	//Z = ZIndex - 1;

	//bit |= m_binaryGrid.Check(X++, Z) ? 0 : 1UL << 3; // msb
	//bit |= m_binaryGrid.Check(X, Z++) ? 0 : 1UL << 2;
	//bit |= m_binaryGrid.Check(X--, Z) ? 0 : 1UL << 1;
	//bit |= m_binaryGrid.Check(X, Z) ? 0 : 1UL; // lsb

	//Vector4 vecPair = lookupMarchingSquares[bit];

	//if (vecPair == Vector4(kZero))
	//	return;

	//Vector2 start = Vector2(vecPair.GetX() + XIndex - 1, vecPair.GetY() + ZIndex - 1);
	//Vector2 end = Vector2(vecPair.GetZ() + XIndex - 1, vecPair.GetW() + ZIndex - 1);

	//for (int i = 0; i < m_contourPoints.size(); i++)
	//{
	//	auto it = std::find(m_contourPoints[i].begin(), m_contourPoints[i].end(), start);
	//	if (it != m_contourPoints[i].end())
	//	{
	//		m_contourPoints[i].push_back(end);
	//		std::cout << "Found a continued line!" << std::endl;
	//		return;
	//	}

	//	it = std::find(m_contourPoints[i].begin(), m_contourPoints[i].end(), end);
	//	if (it != m_contourPoints[i].end())
	//	{
	//		m_contourPoints[i].push_front(start);
	//		std::cout << "Found a continued line!" << std::endl;
	//		return;
	//	}
	//}

	//m_contourPoints.push_back({ start, end });
}