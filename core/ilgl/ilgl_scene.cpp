#include "ilgl_scene.h"
#include "../ew/external/glad.h"
#include <GLFW/glfw3.h>

ilgl::ILGL_Scene::ILGL_Scene()
{
	elements = std::vector<SceneElement>();
	nextID = 0;
}

ilgl::ILGL_Scene::~ILGL_Scene()
{
	for (int i = 0; i < elements.size(); i++)
	{
		delete elements[i].shader;
		delete elements[i].model;
	}
}

int ilgl::ILGL_Scene::addElement( ew::Shader* shader, ew::Model* model, ew::Transform transform, Material mat)
{
	SceneElement newElement = SceneElement();
	newElement.sceneID = getID();
	newElement.shader = shader;
	newElement.model = model;
	newElement.transform = transform;
	newElement.material = mat;

	elements.push_back(newElement);

	return newElement.sceneID;
}

void ilgl::ILGL_Scene::drawScene(ew::Camera eye)
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i].shader->use(); 
		glBindTextureUnit(0, elements[i].material.colorTexture);
		elements[i].shader->setInt("_MainTex", 0);
		glBindTextureUnit(1, elements[i].material.normalTexture);
		elements[i].shader->setInt("_NormalTex", 1);
		elements[i].shader->setMat4("_Model", elements[i].transform.modelMatrix());
		elements[i].shader->setMat4("_ViewProjection", eye.projectionMatrix() * eye.viewMatrix());
		elements[i].shader->setVec3("_EyePos", eye.position);
		elements[i].shader->setFloat("_Material.Ka", elements[i].material.Ka);
		elements[i].shader->setFloat("_Material.Kd", elements[i].material.Kd);
		elements[i].shader->setFloat("_Material.Ks", elements[i].material.Ks);
		elements[i].shader->setFloat("_Material.Shininess", elements[i].material.Shininess);
		elements[i].model->draw();
	}
}

int ilgl::ILGL_Scene::getID()
{
	int givenId = nextID;
	nextID++;
	return givenId;
}
