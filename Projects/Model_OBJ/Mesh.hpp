#pragma once

#include <vector>
#include <string>
#include "meshData.hpp"
#include "../../EsgiGL/EsgiGL.h"
#include "../../EsgiGL/Common/matrix.h"

class Mesh
{
	std::vector<vertexData> data;
	std::vector<textureData> textures;
	std::vector<unsigned short> indices;
	unsigned int vbo;
	unsigned int ind;
public:
	Mesh(std::vector<vertexData> * vd, std::vector<unsigned short> * id, std::vector<textureData> * td = NULL);
	~Mesh();
	void draw(unsigned int programId);
};

