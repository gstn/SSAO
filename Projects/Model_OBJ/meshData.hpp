#pragma once

#include "../ESGIGL/common/vector.h";

struct vertexData {
	vec3 position;
	vec3 normal;
	vec3 tangent;
	vec3 color;
	float u, v;
};

struct textureData {
	unsigned int id;
	unsigned int programId;
	unsigned int type;
	char name[FILENAME_MAX];
};

struct objectTextures {
	textureData diffuse;
	textureData specular;
	textureData normal;
};