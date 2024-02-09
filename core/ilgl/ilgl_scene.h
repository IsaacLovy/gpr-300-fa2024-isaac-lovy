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
		int addElement(ew::Shader* shader, ew::Model* model, ew::Transform transform, Material mat);
		void drawScene(ew::Camera eye);

	private:
		int nextID;
		int getID();
		std::vector<SceneElement> elements;
	};
}