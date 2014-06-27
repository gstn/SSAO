#include "SceneLoader.hpp"

#include "Dist\FreeImage.h"

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

SceneLoader::SceneLoader(Mesh * mesh) {
	meshes.push_back(mesh);
}

void SceneLoader::processMesh(aiMesh * mesh, const aiScene * scene) {
	std::vector<vertexData> data;
	std::vector<unsigned short> indices;
	std::vector<textureData> tex;

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
		textureData td;
		//const char * name = str.C_Str();
		char name[FILENAME_MAX];
		getFilename(str.C_Str(), name);

		std::cout << name << std::endl;

		int index = textureLoadedAt(name);

		if(index < 0) {
			char filepath[FILENAME_MAX];
			strcpy(filepath, folder);
			strcat(filepath, name);
			td.id = loadTexture(filepath);
			td.type = 0;
			strcpy(td.name, name);
			textures.push_back(td);
			index = textures.size() - 1;
		}
		
		tex.push_back(textures[index]);
	}

	meshes.push_back(new Mesh(&data, &indices, &tex));
}

unsigned int SceneLoader::loadTexture(const char * filename) {

	unsigned int textureId = 0;

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename);
	FIBITMAP * bitmap = FreeImage_Load(fif, filename, 0);
	FIBITMAP * pImage = FreeImage_ConvertTo32Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 3.0);
	

	FreeImage_Unload(pImage);
	FreeImage_Unload(bitmap);

	return textureId;
}

SceneLoader::SceneLoader(const char * filename) {

	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(filename, aiProcess_CalcTangentSpace /*|  aiProcess_GenSmoothNormals*/ |aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_SortByPType | aiProcess_FlipUVs | 0);

	if(!scene) {
		std::cout << "Error opening " << filename << std::endl;
		return;
	}
	if(!scene->mFlags || !scene->mRootNode) {
		std::cout << "Error importing " << filename << std::endl;
		return;
	}

	getFolder(filename);

	recursiveProcess(scene->mRootNode, scene);
}


SceneLoader::~SceneLoader(void) {
	for(int i = 0; i < meshes.size(); ++i) {
		delete meshes[i];
	}
}

void SceneLoader::getFolder(const char * filename) {

	int i = 0;
	int index = 0;
	char dir[FILENAME_MAX];

	while(filename[i]) {
		if(filename[i] == '/' || filename[i] == '\\') {
			index = i + 1;
		}
		++i;
	}

	for(i = 0; i < index; ++i) {
		dir[i] = filename[i];
	}

	dir[i] = '\0';

	strcpy(folder, dir);
}

void SceneLoader::getFilename(const char * filepath, char * filename) const {
	int i = 0;
	int j = 0;

	while(filepath[i]) {
		if(filepath[i] == '/' || filepath[i] == '\\') {
			j = 0;
		} else {
			filename[j] = filepath[i];
			++j;
		}
		++i;
	}

	filename[j] = '\0';
}

int SceneLoader::textureLoadedAt(const char * name) const {
	
	for(int i = 0; i < textures.size(); ++i) {
		if(!strcmp(name, textures[i].name)) {
			return i;
		}
	}

	return -1;
}

void SceneLoader::draw(unsigned int programId) {
	for(int i = 0; i < meshes.size(); ++i) {
		meshes[i]->draw(programId);
	}
}

std::vector<Mesh *>& SceneLoader::getMeshes() {
	return meshes;
}