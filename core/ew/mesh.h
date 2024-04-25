/*
*	Author: Eric Winebrenner
*/

#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace ew {
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 tangent;
		float card_ID = 0;
	};

	struct MeshData {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
	};

	enum class DrawMode {
		TRIANGLES = 0,
		POINTS = 1
	};

	class Mesh {
	public:
		Mesh() {};
		Mesh(const MeshData& meshData, bool assignBBox = false);
		void load(const MeshData& meshData, bool generateCardIDs = false);
		void draw(DrawMode drawMode = DrawMode::TRIANGLES)const;
		inline int getNumVertices()const { return m_numVertices; }
		inline int getNumIndices()const { return m_numIndices; }
		void calcBBox(MeshData& meshData);
	private:
		bool m_initialized = false;
		unsigned int m_vao = 0;
		unsigned int m_vbo = 0;
		unsigned int m_ebo = 0;
		unsigned int m_numVertices = 0;
		unsigned int m_numIndices = 0;
		float	min_x, max_x,
				min_y, max_y,
				min_z, max_z;
		glm::vec3 size;
		glm::vec3 center;
	};
}