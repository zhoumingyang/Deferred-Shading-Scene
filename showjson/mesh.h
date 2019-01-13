#pragma once
#ifndef MESH_H
#define MESH_H
#include "model.h"
#include "vao.h"
#include "ibo.h"
#include "texture.h"
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

enum MeshType {
	NORMALMESH,
	OBJMESH
};

class Shader;

struct MeshUnit {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<int> indices;
	MeshUnit(const std::vector<glm::vec3> _vertices, const std::vector<glm::vec2> _uvs,
		const std::vector<glm::vec3> _normals, const std::vector<int> _indices) {
		vertices = _vertices;
		uvs = _uvs;
		normals = _normals;
		indices = _indices;
	}
};

class Mesh {
private:
	VAO* pVao;
	VBO* pVertexBuffer;
	VBO* pUvBuffer;
	VBO* pNormalBuffer;
	IBO* pIbo;
	Texture* pTexture;
	Shader* pShader;
	glm::mat4 modelMat;
	glm::mat4 normalMat;
	std::string objUrl;
	std::string textureUrl;
	std::string normalTextureUrl;
	std::vector<MeshUnit> meshUnits;
	MeshType type;
	void processNode(const aiNode *node, const aiScene *scene);
	MeshUnit processMesh(const aiMesh *mesh, const aiScene *scene);
public:
	Mesh(const Model& model);
	Mesh(const Model& model, Shader* _pShader);
	Mesh(const MeshUnit& meshUnit);
	~Mesh();
	void initBuffer();
	void initTexture();
	void render();
	VAO* getVao() const;
	VBO* getVertexBuffer() const;
	VBO* getUvBuffer() const;
	VBO* getNormalBuffer() const;
	IBO* getIndicesBuffer() const;
	Texture* getTexture() const;
	Shader* getShader() const;
	glm::mat4 getModelMatrix() const;
	glm::mat4 getNormalMatrix() const;
	void setShader(Shader* _pShader);
	MeshType getMeshType() const;
};
#endif // !MESH_H