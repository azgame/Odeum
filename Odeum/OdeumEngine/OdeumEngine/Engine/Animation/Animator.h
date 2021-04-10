#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "../../pch.h"
#include "../Animation/Animation.h"
#include <assimp/scene.h>

struct Animator {
	
	// this function should be goood
	static inline bool ReadSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, Matrix4>>& boneInfoTable)
	{
		// read in data for everybone
		if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end()) // check to see if node is actually a bone
		{
			boneOutput.name = node->mName.C_Str();
			boneOutput.id = boneInfoTable[boneOutput.name].first;
			boneOutput.transform = boneInfoTable[boneOutput.name].second;

			for (int i = 0; i < node->mNumChildren; i++)
			{
				Bone child;
				ReadSkeleton(child, node->mChildren[i], boneInfoTable);
				boneOutput.children.push_back(child);
			}
			return true;
		}
		else // find bones in the children bones
		{
			for (int i = 0; i < node->mNumChildren; i++)
			{
				if (ReadSkeleton(boneOutput, node->mChildren[i], boneInfoTable))
				{
					return true;
				}
			}
		}
		return false;
	};

	// same with this function, maybe make an animation helper class? this function should be good
	static inline void LoadAnimation(const aiScene* scene, Animation& animation)
	{
		// loading the first animation
		aiAnimation* firstAnimation = scene->mAnimations[0];

		if (firstAnimation->mTicksPerSecond != 0.0f)
		{
			animation.ticksPerSecond = firstAnimation->mTicksPerSecond;
		}
		else
		{
			animation.ticksPerSecond = 1;
		}

		animation.duration = firstAnimation->mDuration * firstAnimation->mTicksPerSecond;
		animation.boneTransforms = {};

		// load transformations for each bone (position, rotation, scale)
		// each channel represents each bone
		for (int i = 0; i < firstAnimation->mNumChannels; i++)
		{
			aiNodeAnim* channel = firstAnimation->mChannels[i];
			BoneTransformTrack track;

			for (int j = 0; j < channel->mNumPositionKeys; j++)
			{
				track.positionTimestamps.push_back(channel->mPositionKeys[j].mTime);
				// need to make function to convert assimp vector to our vector
				track.positions.push_back(Math::ASSIMPToVector3(channel->mPositionKeys[j].mValue));
			}
			for (int j = 0; j < channel->mNumRotationKeys; j++)
			{
				track.rotationTimestamps.push_back(channel->mRotationKeys[j].mTime);
				track.rotations.push_back(Math::ASSIMPToQuaternion(channel->mRotationKeys[j].mValue)); // not 100% sure if the assimp to quat works yet
			}
			for (int j = 0; j < channel->mNumScalingKeys; j++)
			{
				track.scaleTimestamps.push_back(channel->mScalingKeys[j].mTime);
				track.scales.push_back(Math::ASSIMPToVector3(channel->mScalingKeys[j].mValue));
			}

			animation.boneTransforms[channel->mNodeName.C_Str()] = track;
		}
	}

	//	COULD BE SOME STUFF HERE THAT NEEDS TO BE TAKEN OUT																									 vv & - I took this out
	static inline void GetPose(Animation& animation, Bone& skeleton, float dt, std::vector<Matrix4>& output, Matrix4 parentTransform, Matrix4& globalInverseTransform)
	{
		BoneTransformTrack& btt = animation.boneTransforms[skeleton.name];
		dt = fmod(dt, animation.duration);
		std::pair<unsigned int, float> fp;

		Vector3 position, scale;
		Quaternion rotation;

		// calculate interpolated position
		if (btt.positionTimestamps.size() > 1) {
			fp = GetTimeFraction(btt.positionTimestamps, dt);
			Vector3 position1 = btt.positions[fp.first - 1];
			Vector3 position2 = btt.positions[fp.first];
			position = Math::Lerp(position1, position2, fp.second);
		}

		// calculated interpolated rotation
		if (btt.rotationTimestamps.size() > 1) {
			fp = GetTimeFraction(btt.rotationTimestamps, dt);
			Quaternion rotation1 = btt.rotations[fp.first - 1];
			Quaternion rotation2 = btt.rotations[fp.first];
			Quaternion rotation = Math::Slerp(rotation1, rotation2, fp.second);
		}

		// calculated interpolated scale
		if (btt.scaleTimestamps.size() > 1) {
			fp = GetTimeFraction(btt.scaleTimestamps, dt);
			Vector3 scale1 = btt.scales[fp.first - 1];
			Vector3 scale2 = btt.scales[fp.first];
			Vector3 scale = Math::Lerp(scale1, scale2, fp.second); // lerp
		}

		Matrix4 positionMat = Matrix4(kIdentity);
		Matrix4 scaleMat = Matrix4(kIdentity);

		// calculated localTransform
		Matrix4 localTransform = Matrix4(DirectX::XMMatrixScalingFromVector(scale) * DirectX::XMMatrixRotationQuaternion(rotation) * DirectX::XMMatrixTranslationFromVector(position));
		Matrix4 globalTransform = parentTransform * localTransform;

		output[skeleton.id] = globalInverseTransform * globalTransform * skeleton.transform;
		
		// update values for children bones
		for (Bone& child : skeleton.children)
		{
			GetPose(animation, child, dt, output, globalTransform, globalInverseTransform);
		}
	}

	static inline std::pair<unsigned int, float> GetTimeFraction(std::vector<float>& times, float& dt) {
		unsigned int segment = 0;
		while (dt > times[segment])
			segment++;
		float start = times[segment - 1]; 
		float end = times[segment];
		float frac = (dt - start) / (end - start);
		return { segment, frac };
	}
};



#endif