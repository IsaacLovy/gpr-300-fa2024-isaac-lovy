#pragma once
#include <vector>
#include "../ew/model.h"
#include "../ew/transform.h"
#include "../ew/camera.h"
#include "../ew/shader.h"

namespace ilgl {
	struct Material {
		float Ka = 0.25;
		float Kd = 0.8;
		float Ks = 0.5;
		float Shininess = 50;
		int colorTexture = 0;
		int normalTexture = 0;
	};

	struct SceneElement
	{
		int sceneID;
		ew::Shader* shader;
		ew::Model* model;
		ew::Transform transform;
		Material material;
	};

	class ILGL_Scene
	{
	public:
		ILGL_Scene();
		~ILGL_Scene();
		int addElement(ew::Shader* shader, ew::Model* mesh, ew::Transform transform, Material mat);
		void setLightDir(glm::vec3 dir);
		void drawScene(ew::Camera eye);
		void drawSceneDepth(ew::Camera eye, ew::Shader globalShader);

	private:
		int nextID;
		int getID();
		glm::vec3 lightDir;
		std::vector<SceneElement> elements;
	};
}