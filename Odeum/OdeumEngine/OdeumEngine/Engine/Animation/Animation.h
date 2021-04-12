#ifndef ANIMATION_H
#define ANIMATION_H

#include "Bone.h"
#include <unordered_map>

struct AnimInfo 
{
	Vector4 boneIDs;
	Vector4 boneWeights;
};

struct Animation 
{
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {}; // keeps track of all the joint movements

	uint32_t m_kNumAnims;
	uint32_t m_boneCount;
	AnimInfo* animInfo;
	StructuredBuffer m_animationBuffer;
};

#endif
