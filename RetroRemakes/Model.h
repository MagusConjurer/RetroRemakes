#pragma once

#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
#include "Texture.h"

#ifndef MODEL
#define MODEL
class Model {
public:
	Model();

	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();

	~Model();

private:
	// TODO: Handle mesh/textures separately and then just reference an ID here
	std::vector<Mesh*> meshList;
	std::vector<Texture*> textureList;
	// Identify which texture from the list to use on a mesh
	std::vector<unsigned int> meshToTexture;

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
};

#endif