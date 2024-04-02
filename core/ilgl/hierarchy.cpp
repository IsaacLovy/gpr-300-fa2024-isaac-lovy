#include "hierarchy.h"

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
