#pragma once


#include <iostream>
#include <map>

#include <Component/SimpleScene.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <Core/Engine.h>
#include "Bone.h"



#define WIN_API_FILE_BROWSING


class Animation : public SimpleScene
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

		std::map<std::string, AnimationNode> boneAnimations;

	public:
		explicit Animation();
		~Animation();
		void Init(aiAnimation* assimpAnimation, std::map<std::string, Bone*>* bones);
		std::string GetName();
		float GetTicksPerSecond();
		float GetDuration();

		glm::vec3 InterpolateScaling(float AnimationTime, const std::string& boneName);
		glm::quat InterpolateRotation(float AnimationTime, const std::string& boneName);
		glm::vec3 InterpolatePosition(float AnimationTime, const std::string& boneName);

};