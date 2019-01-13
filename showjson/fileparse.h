#pragma once
#ifndef FILE_PARSE_H
#define FILE_PARSE_H
#include "model.h"
#include <fstream>
#include "json/json.h"

class FileParse {
private:
	std::string fileName;
	std::vector<Model> models;
	Model parseObjChild(const Json::Value& objChild);
	Model parseMeshChild(const Json::Value& meshChild);
	glm::mat4 createMatrix(const Json::Value& child, const bool& isMesh, const bool& isNormal = false);
public:
	FileParse();
	FileParse(const std::string& _fn);
	~FileParse();
	bool doFileParse();
	std::vector<Model> getModels() const;
	void setModels(const std::vector<Model>& _models);
	
};
#endif // !FILE_PARSE_H