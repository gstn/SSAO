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
	void getFolder(const char * filename);
	void getFilename(const char * filepath, char * filename) const;
	unsigned int loadTexture(const char * filename);
	int textureLoadedAt(const char * name) const;
	char folder[FILENAME_MAX];
public:
	SceneLoader(){}
	SceneLoader(Mesh * mesh);
	SceneLoader(const char * filename);
	~SceneLoader(void);
	void draw(unsigned int programId);
	std::vector<Mesh *>& getMeshes();
	std::vector<textureData> textures;
};

