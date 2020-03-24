#include "Animation.h"

Animation::Animation():
	name("Default"),
	ticksPerSecond(0),
	duration(0),
	boneAnimations()
{

}
Animation::~Animation() {

}

void Animation::Init(aiAnimation* assimpAnimation, std::map<std::string, Bone*>* bones){

	name = (assimpAnimation->mName.C_Str());
	ticksPerSecond = (float)(assimpAnimation->mTicksPerSecond != 0 ? assimpAnimation->mTicksPerSecond : 25.0f);
	duration = (float)(assimpAnimation->mDuration / ticksPerSecond);



	for (std::size_t i = 0; i < assimpAnimation->mNumChannels; ++i) {
		const aiNodeAnim* assimpNodeAnim = assimpAnimation->mChannels[i];

		std::string bone(assimpNodeAnim->mNodeName.data);

		if (bones->find(bone) != bones->end()) {
			
			AnimationNode& animNode = boneAnimations[bone];

			for (unsigned int j = 0; j < assimpNodeAnim->mNumScalingKeys; ++j) {
				ScalingKey key;
				key.time = assimpNodeAnim->mScalingKeys[j].mTime / ticksPerSecond;
				key.scale = glm::vec3(assimpNodeAnim->mScalingKeys[j].mValue.x, assimpNodeAnim->mScalingKeys[j].mValue.y, assimpNodeAnim->mScalingKeys[j].mValue.z);
				animNode.scalingKeys.push_back(key);
			}

			for (unsigned int j = 0; j < assimpNodeAnim->mNumRotationKeys; ++j) {
				RotationKey key;
				key.time = assimpNodeAnim->mRotationKeys[j].mTime / ticksPerSecond;
				glm::quat aux;
				aux.w = assimpNodeAnim->mRotationKeys[j].mValue.w;
				aux.x = assimpNodeAnim->mRotationKeys[j].mValue.x;
				aux.y = assimpNodeAnim->mRotationKeys[j].mValue.y;
				aux.z = assimpNodeAnim->mRotationKeys[j].mValue.z;
				key.rotation = aux;
				animNode.rotationKeys.push_back(key);
			}

			for (unsigned int j = 0; j < assimpNodeAnim->mNumPositionKeys; ++j) {
				PositionKey key;
				key.time = assimpNodeAnim->mPositionKeys[j].mTime / ticksPerSecond;
				key.position = glm::vec3(assimpNodeAnim->mPositionKeys[j].mValue.x, assimpNodeAnim->mPositionKeys[j].mValue.y, assimpNodeAnim->mPositionKeys[j].mValue.z);
				animNode.positionKeys.push_back(key);
			}
		}
	}
}

glm::vec3 Animation::InterpolateScaling(float animationTime, const std::string& boneName) {
	AnimationNode& animation = boneAnimations[boneName];
	animationTime = fmod(animationTime, duration);

	
	unsigned int keyIndex = UINT_MAX;
	if (animation.scalingKeys.size() == 1) {
		return animation.scalingKeys[0].scale;
	}
	for (unsigned int i = 0; i < animation.scalingKeys.size() - 1; ++i) {
		if (animationTime < (float)animation.scalingKeys[i + 1].time) {
			keyIndex = i;
			break;
		}
	}
	if (keyIndex == UINT_MAX) {
		printf("Coudn't find scale key index for animation time %f\n", animationTime);
		return glm::vec3(0, 0, 0);
	}

	unsigned int nextKeyIndex = keyIndex + 1;

	float deltaTime = (float)(animation.scalingKeys[nextKeyIndex].time - animation.scalingKeys[keyIndex].time);
	float factor = (animationTime - (float)animation.scalingKeys[keyIndex].time) / deltaTime;

	if (factor < 0.f || factor > 1.f)
		exit(0);
	
	const glm::vec3& start = animation.scalingKeys[keyIndex].scale;
	const glm::vec3& end = animation.scalingKeys[nextKeyIndex].scale;
	glm::vec3 delta = end - start;

	return start + factor * delta;
}
glm::quat Animation::InterpolateRotation(float animationTime, const std::string& boneName) {
	AnimationNode& animation = boneAnimations[boneName];
	animationTime = fmod(animationTime, duration);

	if (animation.rotationKeys.size() == 1) {
		return animation.rotationKeys[0].rotation;
	}

	unsigned int keyIndex = UINT_MAX;
	for (unsigned int i = 0; i < animation.rotationKeys.size() - 1; ++i) {
		if (animationTime < (float)animation.rotationKeys[i + 1].time) {
			keyIndex = i;
			break;
		}
	}
	if (keyIndex == UINT_MAX) {
		printf("Coudn't find rotation key index for animation time %f\n", animationTime);
		return glm::quat(0, 0, 0, 0);
	}

	unsigned int nextKeyIndex = keyIndex + 1;

//	if (nextKeyIndex > animation.rotationKeys.size())
//		exit(0);

	float deltaTime = (float)(animation.rotationKeys[nextKeyIndex].time - animation.rotationKeys[keyIndex].time);
	float factor = (animationTime - (float)animation.rotationKeys[keyIndex].time) / deltaTime;

//	if (factor < 0.f || factor > 1.f)
//		exit(0);

	const glm::quat& start = animation.rotationKeys[keyIndex].rotation;
	const glm::quat& end = animation.rotationKeys[nextKeyIndex].rotation;

	return glm::slerp(start, end, factor);

}
glm::vec3 Animation::InterpolatePosition(float animationTime, const std::string& boneName) {
	AnimationNode& animation = boneAnimations[boneName];
	animationTime = fmod(animationTime, duration);

	if (animation.positionKeys.size() == 1) {
		return animation.positionKeys[0].position;
	}

	unsigned int keyIndex = UINT_MAX;
	for (unsigned int i = 0; i < animation.positionKeys.size() - 1; ++i) {
		if (animationTime < (float)animation.positionKeys[i + 1].time) {
			keyIndex = i;
			break;
		}
	}
	if (keyIndex == UINT_MAX) {
		printf("Coudn't find position key index for animation time %f\n", animationTime);
		return glm::vec3(0, 0, 0);
	}

	unsigned int nextKeyIndex = keyIndex + 1;
	
//	if (nextKeyIndex > animation.scalingKeys.size())
//		exit(0);
	
	float deltaTime = (float)(animation.positionKeys[nextKeyIndex].time - animation.positionKeys[keyIndex].time);
	float factor = (animationTime - (float)animation.positionKeys[keyIndex].time) / deltaTime;
	
//	if (factor < 0.f || factor > 1.f)
//		exit(0);
	
	const glm::vec3& start = animation.positionKeys[keyIndex].position;
	const glm::vec3& end = animation.positionKeys[nextKeyIndex].position;
	glm::vec3 delta = end - start;

	return start + factor * delta;
}

std::string Animation::GetName() {
	return name;
}
float Animation::GetTicksPerSecond() {
	return ticksPerSecond;
}
float Animation::GetDuration() {
	return duration;
}
