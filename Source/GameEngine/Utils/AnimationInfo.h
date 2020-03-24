#pragma once
#include <GameEngine/Components/Renderers/Animation.h>
#include <GameEngine/Components/Renderers/Model.h>

class AnimationInfo
{
public:
	AnimationInfo(Model* model, Animation* animation, float timestamp) {
		this->model = model;
		this->animation = animation;
		this->timestamp = timestamp;
	}
	~AnimationInfo() {};

	float GetTimeStamp() { return this->timestamp; }
	void SetTimeStamp(float timestamp) { this->timestamp = timestamp; }


	Model* GetModel() { return this->model; }
	void SetModel(Model* model) { this->model = model; }


	Animation* GetAnimation() { return this->animation; }
	void SetAnimation(Animation* animation) { 
		if (this->animation != nullptr) {
			this->animation->SetBlending(false);
			animation->SetPrevAnim(this->animation);
			animation->SetBlending(true);
			animation->SetBlendStartTime(timestamp);
		}
		this->animation = animation;
	}

private:
	Model* model;
	Animation* animation;
	float timestamp;
};