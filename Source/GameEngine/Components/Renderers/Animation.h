#pragma once


#include <iostream>
#include <map>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <Core/Engine.h>
#include "Bone.h"


class Animation
{
	private:
		struct ScalingKey {
			float time;
			glm::vec3 scale;
		};

		struct RotationKey {
			float time;
			glm::quat rotation;
		};

		struct PositionKey {
			float time;
			glm::vec3 position;
		};
		struct AnimationNode {
			std::vector<ScalingKey> scalingKeys;
			std::vector<RotationKey> rotationKeys;
			std::vector<PositionKey> positionKeys;
		};

		std::string name;
		float ticksPerSecond;
		float duration;


		bool isBlending;
		float blendStartTime;
		Animation* prevAnim;

		const float blendingTime = 0.25f;

		std::map<std::string, AnimationNode> boneAnimations;

	public:
		const float GetBlendingTime() { return this->blendingTime; }

		explicit Animation();
		~Animation();
		void Init(aiAnimation* assimpAnimation, std::map<std::string, Bone*>* bones);
		std::string GetName();
		float GetTicksPerSecond();
		float GetDuration();

		void SetBlendStartTime(float startTime) { this->blendStartTime = startTime; }
		float GetBlendStartTime() { return this->blendStartTime; }

		void SetPrevAnim(Animation* prev) { this->prevAnim = prev; }
		Animation* GetPrevAnim() { return this->prevAnim; }

		void SetBlending(bool value) { this->isBlending = value; }
		bool IsBlending() { return this->isBlending; }

		glm::vec3 InterpolateScaling(float AnimationTime, const std::string& boneName);
		glm::quat InterpolateRotation(float AnimationTime, const std::string& boneName);
		glm::vec3 InterpolatePosition(float AnimationTime, const std::string& boneName);

		glm::vec3 BlendScaling(float blendWeight, const std::string& boneName);
		glm::quat BlendRotation(float blendWeight, const std::string& boneName);
		glm::vec3 BlendPosition(float blendWeight, const std::string& boneName);

};