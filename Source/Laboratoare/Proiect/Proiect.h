#pragma once

#include <Component/SimpleScene.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Model.h"
#include "Animation.h"
#include <chrono>
#include <Core/Engine.h>

#define WIN_API_FILE_BROWSING

class Proiect : public SimpleScene
{
	public:
		Proiect();
		~Proiect();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Model* model;
		Model* bob;
		Animation* animation1;
		Animation* animation2;
		Animation* animation3;
		Animation* animation4;
		int whichAnimation;
		float time;
};
