#pragma once

#include <Core/Engine.h>
#include <GameEngine/Components/BaseComponents/BaseComponent.h>

class GraphicsComponent : public BaseComponent
{
	public:

		GraphicsComponent();

		Camera* GetCamera() { return this->camera; }
		void SetCamera(Camera* camera) { this->camera = camera; }

		virtual void update(float deltaTimeSeconds) {};

		virtual void render() {};

	protected:
		Camera* camera;
};