#include "ilgl_scene.h"
#include <GLFW/glfw3.h>

#include "hierarchy.h"

ilgl::ILGL_Scene::ILGL_Scene()
{
	lightDir = glm::vec3(0, -1, 0);
	elements = std::vector<SceneElement>();
	nextID = 0;
}

ilgl::ILGL_Scene::~ILGL_Scene()
{
}

int ilgl::ILGL_Scene::addElement( ew::Shader* shader, ew::Model* model, ew::Transform transform, Material mat)
{
	SceneElement newElement = SceneElement();
	newElement.sceneID = getID();
	newElement.shader = shader;
	newElement.model = model;
	newElement.transform = transform;
	newElement.material = mat;
	newElement.useFK = false;

	elements.push_back(newElement);

	return newElement.sceneID;
}

int ilgl::ILGL_Scene::addElement(ew::Shader* shader, ew::Model* model, ilgl::Node* hierarchyTransform, Material mat)
{
	SceneElement newElement = SceneElement();
	newElement.sceneID = getID();
	newElement.shader = shader;
	newElement.model = model;
	newElement.nodeTransform = hierarchyTransform;
	newElement.material = mat;
	newElement.useFK = true;

	elements.push_back(newElement);

	return newElement.sceneID;
}

void ilgl::ILGL_Scene::setLightDir(glm::vec3 dir)
{
	lightDir = dir;
}

void ilgl::ILGL_Scene::drawScene(ew::Camera eye, ew::Camera lightCam)
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i].shader->use(); 
		glBindTextureUnit(0, elements[i].material.colorTexture);
		elements[i].shader->setInt("_MainTex", 0);
		glBindTextureUnit(1, elements[i].material.normalTexture);
		elements[i].shader->setInt("_NormalTex", 1);
		elements[i].shader->setInt("_FlipbookTexture", 0);


		elements[i].shader->setFloat("_Time", glfwGetTime());
		if (!elements[i].useFK)
		{
			elements[i].shader->setMat4("_Model", elements[i].transform.modelMatrix());
		}
		else
		{
			elements[i].shader->setMat4("_Model", elements[i].nodeTransform->globalTransform);
		}
		elements[i].shader->setMat4("_ViewProjection", eye.projectionMatrix() * eye.viewMatrix());
		elements[i].shader->setMat4("_View", eye.viewMatrix());
		elements[i].shader->setVec3("_EyePos", eye.position);
		elements[i].shader->setVec3("_LightDirection", lightDir);
		elements[i].shader->setMat4("_LightViewProj", lightCam.projectionMatrix() * lightCam.viewMatrix());
		glBindTextureUnit(2, shadowMap);
		elements[i].shader->setInt("_ShadowMap", 2);
		elements[i].shader->setFloat("_MaxBias", shadowMaxBias);
		elements[i].shader->setFloat("_MinBias", shadowMinBias);

		elements[i].shader->setFloat("_Material.Ka", elements[i].material.Ka);
		elements[i].shader->setFloat("_Material.Kd", elements[i].material.Kd);
		elements[i].shader->setFloat("_Material.Ks", elements[i].material.Ks);
		elements[i].shader->setFloat("_Material.Shininess", elements[i].material.Shininess);
		elements[i].model->draw();

	}
}

void ilgl::ILGL_Scene::drawSceneDepth(ew::Camera eye, ew::Shader globalShader)
{
	globalShader.use();
	for (int i = 0; i < elements.size(); i++)
	{
		if (!elements[i].useFK)
		{
			globalShader.setMat4("_Model", elements[i].transform.modelMatrix());
		}
		else
		{
			globalShader.setMat4("_Model", elements[i].nodeTransform->globalTransform);
		}
		globalShader.setMat4("_ViewProjection", eye.projectionMatrix() * eye.viewMatrix());
		elements[i].model->draw();
	}
}

void ilgl::ILGL_Scene::setShadowBuffer(int shadowMap)
{
	this->shadowMap = shadowMap;
}

void ilgl::ILGL_Scene::setShadowBiasMinMax(float min, float max)
{
	shadowMinBias = min;
	shadowMaxBias = max;
}

int ilgl::ILGL_Scene::getID()
{
	int givenId = nextID;
	nextID++;
	return givenId;
}