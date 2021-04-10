#ifndef ANIMATION_H
#define ANIMATION_H

#include "Bone.h"
#include <unordered_map>

struct Animation {
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {}; // keeps track of all the joint movements

	std::vector<Vector4> boneIDs;
	std::vector<Vector4> boneWeights;
};


struct AnimInfo {
	std::vector<Vector4> boneIDs;
	std::vector<Vector4> boneWeights;
	Matrix4 boneTransforms[50];
};
#endif
