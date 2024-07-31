#include "Model.h"

Model::Model() {
}

Model::~Model() {
	ClearModel();
}

bool Model::LoadModel(const std::string& fileName) {
	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices;
	const aiScene* scene = importer.ReadFile(fileName, flags);

	if (!scene) {
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return false;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);

	return true;
}

void Model::RenderModel() {
	for (size_t i = 0; i < meshList.size(); i++) {
		unsigned int materialIndex = meshToTexture[i];

		if (materialIndex < textureList.size() && textureList[materialIndex] != nullptr) {
			textureList[materialIndex]->UseTexture();
		}

		meshList[i]->RenderMesh();
	}
}

void Model::ClearModel() {
	for (size_t i = 0; i < meshList.size(); i++) {
		if (meshList[i]) {
			delete meshList[i];
			meshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < textureList.size(); i++) {
		if (textureList[i]) {
			delete textureList[i];
			textureList[i] = nullptr;
		}
	}
}

void Model::LoadNode(aiNode* node, const aiScene* scene) {
	for (size_t i = 0; i < node->mNumMeshes; i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t j = 0; j < node->mNumChildren; j++) {
		LoadNode(node->mChildren[j], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		// Create the rows with the following pattern : x, y, z, u, v, normals x, normals y, normals z

		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y});
		} else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
	}

	// For now just create a white color array
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		colors.insert(colors.end(), { 1.0f, 1.0f, 1.0f, 1.0f });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &colors[0], &indices[0], vertices.size(), colors.size(), indices.size());
	meshList.push_back(newMesh);
	meshToTexture.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene) {
	textureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		textureList[i] = nullptr;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				// Get just the relative filename in the case of absolute filepath being provided 
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);
				std::string texPath = std::string("Textures/") + filename;

				textureList[i] = new Texture(texPath.c_str());

				if (!textureList[i]->LoadTextureRGB()) {
					printf("Failed to load texture at: %s\n", texPath);

					delete textureList[i];
					textureList[i] = nullptr;
				}
			}
		}

		if (!textureList[i]) {
			textureList[i] = new Texture("Textures/default.png");
			textureList[i]->LoadTextureRGBA();
		}
	}
}






