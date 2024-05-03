#pragma once
#include <vector>
#include "framebuffer.h"
#include "../ew/model.h"
#include "../ew/transform.h"
#include "../ew/camera.h"
#include "../ew/shader.h"
#include <glm/glm.hpp>


namespace ilgl {
	struct Node;
	struct Material {
		float Ka = 0.25;
		float Kd = 0.8;
		float Ks = 0.5;
		float Shininess = 50;
		int colorTexture = 0;
		int normalTexture = 0;

		//Frag Shader 
		glm::vec4 startColor = glm::vec4(0);
		glm::vec4 endColor = glm::vec4(0);

		float opacity = 0.0f;
		float fadeInPower = 0.0f;
		float fadeOutPower = 0.0f;
		float alphaClipThreshhold = 0.0f;

		//Vert Shader

		float flipColumn = 0;
		float flipRow = 0;
		float flipSpeed = 0;
		int matchPhase = 0;

		glm::vec3 gravity = glm::vec3(0);
		glm::vec3 wind = glm::vec3(0);

		glm::vec3 emmiterDimensions = glm::vec3(0);
		float rotSpeed = 0.0f;
		float rotation = 0.0f;
		int rotRandomOffset = 0;
		int rotRandomDirection = 0;

		float particleSpeed = 0.0f;
		glm::vec3 particleDirMin = glm::vec3(0);
		glm::vec3 particleDirMax = glm::vec3(0);
		float particleSpread = 0;
		float particleVelStart = 0;
		float particleVelEnd = 0;

		float particleStartSize = 0;
		float particleEndSize = 0;

		Material() {};

		Material(glm::vec4 startColor, glm::vec4 endColor, int flipBookTex, float opacity, float fadeInPower,
			float fadeOutPower, float alphaClipThreshhold, float flipColumn, float flipRow, float flipSpeed,
			int matchPhase, glm::vec3 gravity, glm::vec3 wind, glm::vec3 emmiterDimensions, float rotSpeed,
			float rotation, int rotRandomOffset, int rotRandomDirection, float particleSpeed,
			glm::vec3 particleDirMin, glm::vec3 particleDirMax, float particleSpread, float particleVelStart,
			float particleVelEnd, float particleStartSize, float particleEndSize)
			: startColor(startColor), endColor(endColor), colorTexture(flipBookTex), opacity(opacity),
			fadeInPower(fadeInPower), fadeOutPower(fadeOutPower), alphaClipThreshhold(alphaClipThreshhold),
			flipColumn(flipColumn), flipRow(flipRow), flipSpeed(flipSpeed), matchPhase(matchPhase),
			gravity(gravity), wind(wind), emmiterDimensions(emmiterDimensions), rotSpeed(rotSpeed),
			rotation(rotation), rotRandomOffset(rotRandomOffset), rotRandomDirection(rotRandomDirection),
			particleSpeed(particleSpeed), particleDirMin(particleDirMin),
			particleDirMax(particleDirMax), particleSpread(particleSpread), particleVelStart(particleVelStart),
			particleVelEnd(particleVelEnd), particleStartSize(particleStartSize),
			particleEndSize(particleEndSize) {}

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

		void setMaterial(int id, Material newMat);



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