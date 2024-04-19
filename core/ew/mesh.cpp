/*
	Author: Eric Winebrenner
*/
/*
*	Author: Eric Winebrenner
*/

#include "mesh.h"
#include "external/glad.h"

namespace ew {
	Mesh::Mesh(const MeshData& meshData)
	{
		load(meshData);
	}
	void Mesh::load(const MeshData& meshData)
	{
		if (!m_initialized) {
			glGenVertexArrays(1, &m_vao);
			glBindVertexArray(m_vao);

			glGenBuffers(1, &m_vbo);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

			glGenBuffers(1, &m_ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
			//Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
			glEnableVertexAttribArray(0);

			//Normal attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));
			glEnableVertexAttribArray(1);

			//UV attribute
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, uv)));
			glEnableVertexAttribArray(2);

			//Tangent attribute
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, tangent));
			glEnableVertexAttribArray(3);

			m_initialized = true;
		}

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		if (meshData.vertices.size() > 0) {
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * meshData.vertices.size(), meshData.vertices.data(), GL_STATIC_DRAW);
		}
		if (meshData.indices.size() > 0) {
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshData.indices.size(), meshData.indices.data(), GL_STATIC_DRAW);
		}
		m_numVertices = meshData.vertices.size();
		m_numIndices = meshData.indices.size();

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	void Mesh::draw(ew::DrawMode drawMode) const
	{
		glBindVertexArray(m_vao);
		if (drawMode == DrawMode::TRIANGLES) {
			glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, NULL);
		}
		else {
			glDrawArrays(GL_POINTS, 0, m_numVertices);
		}
		
	}
	void Mesh::calcBBox(MeshData& meshData)
	{
		float	min_x, max_x,
				min_y, max_y,
				min_z, max_z;

		min_x = max_x = meshData.vertices[0].pos.x;
		min_y = max_y = meshData.vertices[0].pos.y;
		min_z = max_z = meshData.vertices[0].pos.z;

		for (int i = 0; i < meshData.vertices.size(); i++)
		{
			min_x = std::min(min_x, meshData.vertices[i].pos.x);
			min_y = std::min(min_y, meshData.vertices[i].pos.y);
			min_z = std::min(min_z, meshData.vertices[i].pos.z);

			max_x = std::min(max_x, meshData.vertices[i].pos.x);
			max_y = std::min(max_y, meshData.vertices[i].pos.y);
			max_z = std::min(max_z, meshData.vertices[i].pos.z);
		}
	}
}