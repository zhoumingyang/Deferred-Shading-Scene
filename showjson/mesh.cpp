#include "mesh.h"
#include "image.h"
#include "shader.h"

Mesh::Mesh(const Model& model) {
	if (model.isMesh) {
		meshUnits.push_back(MeshUnit(model.vertexPos, model.vertexUvs, model.vertexNormals, model.vertexTris));
		type = NORMALMESH;
	}
	else {
		type = OBJMESH;
	}
	modelMat = model.matrix;
	normalMat = model.normalMatrix;
	objUrl = model.objUrl;
	textureUrl = model.textureUrl;
	normalTextureUrl = model.textureUrl;
	color = glm::vec3(1.0, 1.0, 1.0);
	pVao = NULL;
	pVertexBuffer = NULL;
	pUvBuffer = NULL;
	pNormalBuffer = NULL;
	pIbo = NULL;
	pTexture = NULL;
	pShader = NULL;

	pShader = NULL;
}

Mesh::Mesh(const Model& model, Shader* _pShader) {
	if (model.isMesh) {
		meshUnits.push_back(MeshUnit(model.vertexPos, model.vertexUvs, model.vertexNormals, model.vertexTris));
		type = NORMALMESH;
	}
	else {
		type = OBJMESH;
	}

	modelMat = model.matrix;
	normalMat = model.normalMatrix;
	objUrl = model.objUrl;
	textureUrl = model.textureUrl;
	normalTextureUrl = model.textureUrl;
	color = glm::vec3(1.0, 1.0, 1.0);
	pVao = NULL;
	pVertexBuffer = NULL;
	pUvBuffer = NULL;
	pNormalBuffer = NULL;
	pIbo = NULL;
	pTexture = NULL;
	pShader = NULL;

	pShader = _pShader;
}

Mesh::Mesh(const MeshUnit& meshUnit) {
	meshUnits.push_back(meshUnit);
	color = glm::vec3(1.0, 1.0, 1.0);
	type = NONE;
	modelMat = glm::mat4(1.0);
	normalMat = glm::mat4(1.0);
}

Mesh::Mesh(const ShapeGeometry& shape) {
	MeshUnit tmpMeshUnit(shape.getVertices(), shape.getUvs(), shape.getNormals(), shape.getIndices());
	meshUnits.push_back(tmpMeshUnit);
	type = AIDMESH;
	modelMat = shape.getModelMatrix();
	normalMat = shape.getNormalMatrix();
	color = shape.getColor();
}

Mesh::~Mesh() {
	if (pVao != NULL) {
		delete pVao;
		pVao = NULL;
	}
	if (pVertexBuffer != NULL) {
		delete pVertexBuffer;
		pVertexBuffer = NULL;
	}
	if (pUvBuffer != NULL) {
		delete pUvBuffer;
		pUvBuffer = NULL;
	}
	if (pNormalBuffer != NULL) {
		delete pNormalBuffer;
		pNormalBuffer = NULL;
	}
	if (pTexture != NULL) {
		delete pTexture;
		pTexture = NULL;
	}
	if (pShader != NULL) {
		delete pShader;
		pShader = NULL;
	}
}

MeshUnit Mesh::processMesh(const aiMesh *mesh, const aiScene *scene) {
	std::vector<glm::vec3> tmpVertices;
	std::vector<glm::vec2> tmpUvs;
	std::vector<glm::vec3> tmpNormals;
	std::vector<int> tmpIndices;
	for (int i = 0; i < mesh->mNumVertices; i++) {
		tmpVertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		if (mesh->mTextureCoords[0]) {
			tmpUvs.push_back(glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
		}
		else {
			tmpUvs.push_back(glm::vec2(0, 0));
		}
		tmpNormals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			tmpIndices.push_back(face.mIndices[j]);
		}	
	}
	return MeshUnit(tmpVertices, tmpUvs, tmpNormals, tmpIndices);
}

void Mesh::processNode(const aiNode *node, const aiScene *scene) {
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::string meshName = mesh->mName.C_Str();
		if (meshName != "g shadow") {
			meshUnits.push_back(processMesh(mesh, scene));
		}
	}
	for (int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

void Mesh::initBuffer() {
	if (objUrl.size() > 0) {
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(objUrl, aiProcess_Triangulate);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ERROR::ASSIMP: " << importer.GetErrorString() << std::endl;
			return;
		}
		processNode(scene->mRootNode, scene);
	}
	pVao = new VAO(meshUnits.size());
	pVertexBuffer = new VBO(meshUnits.size());
	pUvBuffer = new VBO(meshUnits.size());
	pNormalBuffer = new VBO(meshUnits.size());
	pIbo = new IBO(meshUnits.size());

	for (int i = 0; i < meshUnits.size(); i++) {
		pVertexBuffer->setBufferData(i, &meshUnits[i].vertices[0], meshUnits[i].vertices.size() * sizeof(glm::vec3));
		pVertexBuffer->unBind();

		pUvBuffer->setBufferData(i, &meshUnits[i].uvs[0], meshUnits[i].uvs.size() * sizeof(glm::vec2));
		pUvBuffer->unBind();

		pNormalBuffer->setBufferData(i, &meshUnits[i].normals[0], meshUnits[i].normals.size() * sizeof(glm::vec3));
		pNormalBuffer->unBind();

		pVao->bind(i);
		pVao->setAttribPoint(*pVertexBuffer, i, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
		pVao->setAttribPoint(*pUvBuffer, i, 1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)0);
		pVao->setAttribPoint(*pNormalBuffer, i, 2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
		pIbo->setElementData(i, &meshUnits[i].indices[0], meshUnits[i].indices.size() * sizeof(int));
		pIbo->unBind();
		pVao->unBind();

		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			std::cout << "some thing error: " << err << std::endl;
			getchar();
			exit(1);
		}
	}
}

void Mesh::initTexture() {
	if (textureUrl.size()) {
		Image* image = new Image(textureUrl);
		if (!image->loadImage()) {
			std::cout << "load image error" << std::endl;
			return;
		}
		GLenum format = image->getImageComponents();
		pTexture = new Texture();
		pTexture->bind(0);
		pTexture->setTextureData(0, image->getImageData(), format, image->getWidth(), image->getHeight(), format);
		pTexture->setWrapMode(GL_TEXTURE_WRAP_S, GL_REPEAT);
		pTexture->setWrapMode(GL_TEXTURE_WRAP_T, GL_REPEAT);
		pTexture->setFilterMode(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		pTexture->setFilterMode(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		pTexture->unBind();
		delete image;
	}
	if (normalTextureUrl.size()) {
		//std::cout << "TODO! normal map" << std::endl;
	}
}

void Mesh::render() {
	if (pShader != NULL) {
		pShader->use();
	}
	for (int i = 0; i < meshUnits.size(); i++) {
		if (pTexture != NULL) {
			pTexture->active(0, 0);
		}
		pVao->bind(i);
		pIbo->bind(i);
		glDrawElements(GL_TRIANGLES, meshUnits[i].indices.size(), GL_UNSIGNED_INT, 0);
		pVao->unBind();
	}
	if (pShader != NULL) {
		pShader->unuse();
	}
}

VAO* Mesh::getVao() const {
	return pVao;
}

VBO* Mesh::getVertexBuffer() const {
	return pVertexBuffer;
}

VBO* Mesh::getNormalBuffer() const {
	return pNormalBuffer;
}

VBO* Mesh::getUvBuffer() const {
	return pUvBuffer;
}

IBO* Mesh::getIndicesBuffer() const {
	return pIbo;
}

Texture* Mesh::getTexture() const {
	return pTexture;
}

Shader* Mesh::getShader() const {
	return pShader;
}

glm::mat4 Mesh::getModelMatrix() const {
	return modelMat;
}

glm::mat4 Mesh::getNormalMatrix() const {
	return normalMat;
}

void Mesh::setShader(Shader* _pShader) {
	pShader = _pShader;
}

MeshType Mesh::getMeshType() const {
	return type;
}

glm::vec3 Mesh::getColor() const {
	return color;
}