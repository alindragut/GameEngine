#include "ShadowMapSceneRenderer.h"
#include <GameEngine/Components/ComponentsIncludeList.h>
#include <GameEngine/IncludeList.h>
#include <GameEngine/ImGUI/imgui.h>

ShadowMapSceneRenderer::ShadowMapSceneRenderer(std::list<BaseGameObject*>* objects) {
	this->objects = objects;
	lightPos = glm::vec3(0);
	dirLightView = glm::mat4(1);
	dirLightProj = glm::mat4(1);
}

ShadowMapSceneRenderer::~ShadowMapSceneRenderer() {
	delete defaultPass;
	delete pointShadowPass;
	delete dirShadowPass;
}

void ShadowMapSceneRenderer::Init(unsigned int scene_width, unsigned int scene_height,
								  unsigned int point_shadow_width, unsigned int point_shadow_height,
								  unsigned int dir_shadow_width, unsigned int dir_shadow_height) {
	defaultPass = new DefaultRenderPass();
	dirShadowPass = new ShadowMapPass();
	pointShadowPass = new CubeShadowMapPass();
	defaultPass->Init(scene_width, scene_height);
	pointShadowPass->Init(point_shadow_width, point_shadow_height);
	dirShadowPass->Init(dir_shadow_width, dir_shadow_height);
	shadowAspectRatio = (float)point_shadow_width / (float)point_shadow_height;
}

void ShadowMapSceneRenderer::renderScene() {
	pointShadowPass->BindForWriting();
	/*static float hh = 30.0f;
	static float ww = 30.0f;
	ImGui::SliderFloat("height", &hh, 0.0f, 100.0f);*/
	//ImGui::SliderFloat("bias", &x, -0.2f, 0.2f);
	//ImGui::SliderFloat("far", &y, 5.0f, 200.0f);
	Camera* cam = EngineManager::GetInstance().GetGameEngine()->GetCamera();
	glm::vec3 prevPos = cam->transform->GetWorldPosition();
	glm::quat prevRot = cam->transform->GetWorldRotation();
	float oldFov = cam->GetProjectionInfo().FoVy;
	float farPlane = cam->GetProjectionInfo().zFar;
	float nearPlane = cam->GetProjectionInfo().zNear;
	float aspect = cam->GetProjectionInfo().aspectRatio;
	cam->SetPerspective(90, shadowAspectRatio, nearPlane, farPlane);
	/**/
	cam->Update();
	renderPointShadow();

	dirShadowPass->BindForWriting();
	cam->SetOrthographic(2, 2, 0.01f, 5);
	cam->transform->SetWorldPosition(lightPos + glm::vec3(1));
	cam->transform->SetWorldRotation(glm::inverse(glm::lookAt(glm::vec3(lightPos) + glm::vec3(1), glm::vec3(lightPos), glm::vec3(0, 1, 0))));
	cam->Update();
	dirLightView = cam->GetViewMatrix();
	dirLightProj = cam->GetProjectionMatrix();
	renderDirShadow();

	defaultPass->BindForWriting();
	pointShadowPass->BindForReading(GL_TEXTURE1);
	dirShadowPass->BindForReading(GL_TEXTURE2);

	cam->SetPerspective(oldFov, aspect, nearPlane, farPlane);
	cam->transform->SetWorldPosition(prevPos);
	cam->transform->SetWorldRotation(prevRot);
	cam->Update();
	renderGameObjects();
}

void ShadowMapSceneRenderer::renderGameObjects() {
	for (auto it = (*objects).begin(); it != (*objects).end();) {
		if (AnimationRenderer* ar = dynamic_cast<AnimationRenderer*>(((*it)->GetComponent("AnimationRenderer")))) {
			ar->render();
		}
		if (DefaultRenderer* dr = dynamic_cast<DefaultRenderer*>(((*it)->GetComponent("DefaultRenderer")))) {
			dr->render();
		}
		if (TextureRenderer* tr = dynamic_cast<TextureRenderer*>(((*it)->GetComponent("TextureRenderer")))) {
			tr->render();
		}
		if (PointShadowRenderer* psr = dynamic_cast<PointShadowRenderer*>(((*it)->GetComponent("PointShadowRenderer")))) {
			psr->SetRenderDepth(false);
			psr->SetShader("PointShadowLight");
			psr->SetDirLightView(dirLightView);
			psr->SetDirLightProj(dirLightProj);
			psr->render();
		}
		++it;
	}
}

void ShadowMapSceneRenderer::renderPointShadow() {
	for (auto it = (*objects).begin(); it != (*objects).end();) {
		if (PointLightComponent* plc = dynamic_cast<PointLightComponent*>(((*it)->GetComponent("PointLightComponent")))) {
			lightPos = plc->GetWorldPosition() + glm::vec3(0, 0.5, 0);
		}
		if (PointShadowRenderer* psr = dynamic_cast<PointShadowRenderer*>(((*it)->GetComponent("PointShadowRenderer")))) {
			psr->SetRenderDepth(true);
			psr->SetPos(lightPos);

			if (!psr->isSkybox()) {
				psr->SetShader("PointShadowDepth");
				psr->render();
			}
		}
		++it;
	}
}

void ShadowMapSceneRenderer::renderDirShadow() {
	for (auto it = (*objects).begin(); it != (*objects).end();) {
		if (AnimationRenderer* ar = dynamic_cast<AnimationRenderer*>(((*it)->GetComponent("AnimationRenderer")))) {
			ar->render();
		}
		++it;
	}
}