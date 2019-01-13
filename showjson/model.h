#pragma once
#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <string>
#include <vector>
#include "glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/common.hpp"
extern struct Model {
	std::vector<glm::vec3> vertexPos;
	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec2> vertexUvs;
	std::vector<int> vertexTris;
	glm::mat4x4 matrix;
	glm::mat4x4 normalMatrix;
	std::string textureUrl;
	std::string normalTextureUrl;
	std::string objUrl;
	bool isMesh;
};
#endif // !MODEL_H
