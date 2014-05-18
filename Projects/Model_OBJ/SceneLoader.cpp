#include "SceneLoader.hpp"

void SceneLoader::recursiveProcess(aiNode * node, const aiScene * scene) {

	//process
	for(int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}

	//recursion
	for(int i = 0; i < node->mNumChildren; ++i) {
		recursiveProcess(node->mChildren[i], scene);
	}
}

void SceneLoader::processMesh(aiMesh * mesh, const aiScene * scene) {
	std::vector<vertexData> data;
	std::vector<unsigned int> indices;
	std::vector<textureData> textures;

	for(int i = 0; i < mesh->mNumVertices; ++i) {
		vertexData tmp;
		vec3 tmpVec;

		//position
		tmpVec.x = mesh->mVertices[i].x;
		tmpVec.y = mesh->mVertices[i].y;
		tmpVec.z = mesh->mVertices[i].z;
		tmp.position = tmpVec;

		//normals
		tmpVec.x = mesh->mNormals[i].x;
		tmpVec.y = mesh->mNormals[i].y;
		tmpVec.z = mesh->mNormals[i].z;
		tmp.normal = tmpVec;

		if(mesh->mTangents) {
			tmpVec.x = mesh->mTangents[i].x;
			tmpVec.y = mesh->mTangents[i].y;
			tmpVec.z = mesh->mTangents[i].z;
		} else {
			tmpVec.x = 1.0;
			tmpVec.y = tmpVec.z = 0;
		}
		tmp.tangent = tmpVec;

		if(mesh->mColors[0]) {
			tmpVec.x = mesh->mColors[0][i].r;
			tmpVec.y = mesh->mColors[0][i].g;
			tmpVec.z = mesh->mColors[0][i].b;
		} else {
			tmpVec.x = tmpVec.y = tmpVec.z = 0.7;
		}
		tmp.color = tmpVec;

		if(mesh->mTextureCoords[0]) {
			tmpVec.x = mesh->mTextureCoords[0][i].x;
			tmpVec.y = mesh->mTextureCoords[0][i].y;
			tmpVec.z = mesh->mTextureCoords[0][i].z;
		} else {
			tmpVec.x = tmpVec.y = tmpVec.z = 0.0;
		}
		tmp.u = tmpVec.x;
		tmp.v = tmpVec.y;

		data.push_back(tmp);
	}

	for(int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for(int j = 0; j < face.mNumIndices; ++j) {
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial * mat = scene->mMaterials[mesh->mMaterialIndex];
	for(int i = 0; i < mat->GetTextureCount(aiTextureType_DIFFUSE); ++i) {
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
		textureData tmp;
		tmp.id = loadTexture(str.C_Str());
		tmp.type = 0;
		textures.push_back(tmp);
	}

	meshes.push_back(new Mesh(&data, &indices, &textures));
}

unsigned int SceneLoader::loadTexture(const char * filename) {
	return 0;
}

SceneLoader::SceneLoader(const char * filename) {
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace |  aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_SortByPType | aiProcess_FlipUVs | 0);
	if(!scene) {
		std::cout << "Error opening " << filename << std::endl;
		return;
	}
	if(!scene->mFlags || !scene->mRootNode) {
		std::cout << "Error importing " << filename << std::endl;
		return;
	}
	recursiveProcess(scene->mRootNode, scene);
}


SceneLoader::~SceneLoader(void) {
	for(int i = 0; i < meshes.size(); ++i) {
		delete meshes[i];
	}
}

void SceneLoader::draw(unsigned int programId) {
	for(int i = 0; i < meshes.size(); ++i) {
		meshes[i]->draw(programId);
	}
}

std::vector<Mesh *>& SceneLoader::getMeshes() {
	return meshes;
}