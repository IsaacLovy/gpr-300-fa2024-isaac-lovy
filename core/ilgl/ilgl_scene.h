#pragma once
#include <vector>
#include "framebuffer.h"
#include "../ew/model.h"
#include "../ew/transform.h"
#include "../ew/camera.h"
#include "../ew/shader.h"


namespace ilgl {
	struct Node;
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
		bool useFK;
		Node* nodeTransform;
		Material material;
	};

	class ILGL_Scene
	{
	public:
		ILGL_Scene();
		~ILGL_Scene();
		int addElement(ew::Shader* shader, ew::Model* mesh, ew::Transform transform, Material mat);
		int addElement(ew::Shader* shader, ew::Model* mesh, Node* hierarchyTransform, Material mat);
		void setLightDir(glm::vec3 dir);
		void drawScene(ew::Camera eye, ew::Camera lightCam);
		void drawSceneDepth(ew::Camera eye, ew::Shader shadowShader);
		void setShadowBuffer(int shadowMap);
		void setShadowBiasMinMax(float min, float max);
		unsigned int createCubeMap(std::vector<std::string> faces);

	private:
		int nextID;
		int getID();
		glm::vec3 lightDir;
		int shadowMap;
		float shadowMinBias = 0.005;
		float shadowMaxBias = 0.015;
		std::vector<SceneElement> elements;
	};
}