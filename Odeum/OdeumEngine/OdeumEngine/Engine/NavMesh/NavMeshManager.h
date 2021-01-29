#ifndef NAVMESHMANAGER_H
#define MAVMESHMANAGER_H

#include "../../pch.h"

struct Collider2D;
class Plane;

namespace NavMeshManager
{
	void Initialize(float CellSize);
	void GenerateNavMesh( /*Smaller is more precise*/ float CellSize, Plane GroundPlane, std::vector<Collider2D> Obstacles);
	void Uninitialize();
}

#endif