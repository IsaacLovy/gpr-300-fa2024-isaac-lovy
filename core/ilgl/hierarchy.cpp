#include "hierarchy.h"

ilgl::Hierarchy::~Hierarchy()
{
	delete[] nodes;
}

void ilgl::Hierarchy::setup(Node* newHierarchy, int numNodes)
{
	nodeCount = numNodes;

	nodes = new Node[nodeCount];

	for (int i = 0; i < nodeCount; i++)
	{
		nodes[i] = newHierarchy[i];
	}
}

void ilgl::Hierarchy::addToScene(ILGL_Scene* scene, ew::Shader* shader, ew::Model* mesh, Material mat)
{
	for (int i = 0; i < nodeCount; i++)
	{
		scene->addElement(shader, mesh, &(nodes[i]), mat);
	}
}

void ilgl::Hierarchy::updateNodeRot(int id, glm::vec3 axis, float degrees, bool absolute)
{
	glm::quat rotation = glm::angleAxis(glm::radians(degrees), axis);

	if (!absolute)
	{
		nodes[id].rotation *= rotation;
	}
	else
	{
		nodes[id].rotation *= rotation;
	}
}

void ilgl::Hierarchy::solveFK()
{
	for( int i = 0; i < nodeCount; i ++)
	{
		if (nodes[i].parentIndex == -1)
		{
			nodes[i].globalTransform = nodes[i].localTransform();
		}
		else
		{
			nodes[i].globalTransform = nodes[nodes[i].parentIndex].globalTransform * nodes[i].localTransform();
		}
	}
}
