#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../ew/transform.h"
#include "../ew/model.h"
#include "../ew/shader.h"
#include "ilgl_scene.h"

namespace ilgl {

	struct Node {
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f,0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::mat4 globalTransform;

		unsigned int parentIndex = -1;

		glm::mat4 localTransform() const {
			glm::mat4 m = glm::mat4(1.0f);
			m = glm::translate(m, position);
			m *= glm::mat4_cast(rotation);
			m = glm::scale(m, scale);
			return m;
		}
	};


	struct Hierarchy
	{
		Node* nodes;
		unsigned int nodeCount;

		void setup(Node* newHierarchy, int numNodes);
		void addToScene(ILGL_Scene* scene, ew::Shader* shader, ew::Model* mesh, Material mat);
		void solveFK();
	};
}