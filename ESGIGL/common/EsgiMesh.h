// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

#ifndef ESGI_SCENE_MESH
#define ESGI_SCENE_MESH

// --- Includes --------------------------------------------------------------

#include "matrix.h"
#include <vector>
#include <string>

// --- Classes ---------------------------------------------------------------

class EsgiMesh
{
public:
	EsgiMesh() {}
	~EsgiMesh() {}

	bool LoadObj(const char *path);
	void Unload();
	
	inline const vec3* GetVertices() const			{ return &m_vertices[0]; }
	inline const vec3* GetNormals() const			{ return &m_normals[0]; }
	inline const vec2* GetTexCoords() const			{ return &m_texCoords[0]; }
	inline const unsigned short* GetIndices() const	{ return &m_indices[0]; }

	inline unsigned int GetVertexCount() const		{ return m_vertices.size(); }
	inline unsigned int GetIndexCount() const		{ return m_indices.size(); }

private:
	std::string						m_name;
	std::vector<vec3>				m_vertices;
	std::vector<vec3>				m_normals;
	std::vector<vec2>				m_texCoords;
	std::vector<unsigned short>		m_indices;

	unsigned short					m_triangleCount;
};

#endif // ESGI_SCENE_MESH