#ifndef BONE_H
#define BONE_H

//#include "../../pch.h"
#include "../Math/Matrix.h"

struct Bone {
	int id = 0;
	std::string name = "";
	Matrix4 transform = Matrix4(kIdentity);

	std::vector<Bone> children = {}; // all the children bones of this bone
};

struct BoneTransformTrack {
	// these vectors contain the timestamps of when to perform translation, rotation, or scale
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	// these vectors contain the actual modifications we want to make to the bone
	// timestamps[i] should match with modifications[i]
	std::vector<Vector3> positions = {};
	std::vector<Quaternion> rotations = {};
	std::vector<Vector3> scales = {};
};

#endif