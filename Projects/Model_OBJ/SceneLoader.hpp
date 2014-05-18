#pragma once

#include <iostream>
#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.hpp"

class SceneLoader
{
	std::vector<Mesh*> meshes;
	void recursiveProcess(aiNode * node, const aiScene * scene);
	void processMesh(aiMesh * mesh, const aiScene * scene);
	unsigned int loadTexture(const char * filename);
public:
	SceneLoader(const char * filename);
	~SceneLoader(void);
	void draw(unsigned int programId);
	std::vector<Mesh *>& getMeshes();
};

