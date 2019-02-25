#include "jsonconfig.h"
#include "fileparse.h"
#include <urlmon.h>
#pragma comment(lib, "urlmon.lib")

void setVertexAttribute(const Json::Value& vertexInfos, std::vector<glm::vec3>& attributes, bool ifFloat, bool isVertex) {
	for (int j = 0; j + 2 < vertexInfos.size(); j += 3) {
		glm::vec3 info(ifFloat ? (isVertex ? vertexInfos[j].asFloat() * 1.0 : vertexInfos[j].asFloat()) : vertexInfos[j].asInt(),
			ifFloat ? (isVertex ? vertexInfos[j + 1].asFloat() * 1.0 : vertexInfos[j + 1].asFloat()) : vertexInfos[j + 1].asInt(),
			ifFloat ? (isVertex ? vertexInfos[j + 2].asFloat() * 1.0 : vertexInfos[j + 2].asFloat()) : vertexInfos[j + 2].asInt());
		attributes.push_back(info);
	}
}

void setVertexAttribute(const Json::Value& vertexInfos, std::vector<glm::vec2>& attributes) {
	for (int j = 0; j + 1 < vertexInfos.size();j += 2) {
		glm::vec2 uv(vertexInfos[j].asFloat(), vertexInfos[j + 1].asFloat());
		attributes.push_back(uv);
	}
}

FileParse::FileParse() {
	fileName = "./design/scene-oneroom.json";
}

FileParse::FileParse(const std::string& fn) {
	fileName = fn;
}

FileParse::~FileParse() {

}

Model FileParse::parseObjChild(const Json::Value& objChild) {
	Model model;
	std::string modelUrl = objChild[JSON_MODEL_PATH].asString();
	std::string imageUrl = objChild[JSON_IMG_PATH].asString();
	std::string modelLocalPath = modelUrl;
	std::string imageLocalPath = imageUrl;
	if (modelUrl.size() > 0 && modelUrl.find("http") != std::string::npos) {
		modelLocalPath = "./obj/" + objChild[JSON_MODEL_NAME].asString()+ objChild[JSON_ID].asString() + "_model.obj";
		HRESULT resModel = URLDownloadToFile(NULL, modelUrl.c_str(), modelLocalPath.c_str(), 0, NULL);
	}
	if (imageUrl.size() > 0 && imageUrl.find("http") != std::string::npos) {
		imageLocalPath = "./res/" + objChild[JSON_MODEL_NAME].asString() + objChild[JSON_ID].asString() + "_model.png";
		HRESULT resImage = URLDownloadToFile(NULL, imageUrl.c_str(), imageLocalPath.c_str(), 0, NULL);
	}
	model.isMesh = false;
	model.objUrl = modelLocalPath;
	model.textureUrl = imageLocalPath;
	return model;
}

Model FileParse::parseMeshChild(const Json::Value& meshChild) {
	Model model;
	model.isMesh = true;
	Json::Value vertices = meshChild[JSON_POINTS];
	Json::Value normals = meshChild[JSON_NORAMLS];
	Json::Value uvs = meshChild[JSON_UVS];
	Json::Value indices = meshChild[JSON_INDICES];
	setVertexAttribute(vertices, model.vertexPos, true, true);
	setVertexAttribute(normals, model.vertexNormals, true, false);
	setVertexAttribute(uvs, model.vertexUvs);
	if (indices.size() > 0) {
		for (int j = 0; j < indices.size(); j++) {
			model.vertexTris.push_back(indices[j].asInt());
		}
	}
	else {
		for (int j = 0; j < model.vertexPos.size(); j++) {
			model.vertexTris.push_back(j);
		}
	}
	model.textureUrl = meshChild[JSON_IMG_PATH].asString();
	model.normalTextureUrl = meshChild[JSON_NORAML_IMG_PATH].asString();
	if (model.textureUrl.size() > 0 && model.textureUrl.find("http") != std::string::npos) {
		std::string localPath = "./res/" + meshChild[JSON_MESH_NAME].asString()+ meshChild[JSON_ID].asString() + ".jpg";
		HRESULT res = URLDownloadToFile(NULL, model.textureUrl.c_str(), localPath.c_str(), 0, NULL);
		model.textureUrl = localPath;
	}
	else if (model.textureUrl.size() > 0) {
		std::string localPath = "./" + model.textureUrl;
		model.textureUrl = localPath;
	}
	if (model.normalTextureUrl.size() > 0 && model.normalTextureUrl.find("http") != std::string::npos) {
		std::string localPath = "./res/" + meshChild[JSON_MESH_NAME].asString() + meshChild[JSON_ID].asString() + "_normal" + ".jpg";
		HRESULT res = URLDownloadToFile(NULL, model.normalTextureUrl.c_str(), localPath.c_str(), 0, NULL);
		model.normalTextureUrl = localPath;
	}
	return model;
}

glm::mat4 FileParse::createMatrix(const Json::Value& child, const bool& isMesh, const bool& isNormal) {
	Json::Value pos = child[JSON_POS];
	Json::Value rot = child[JSON_ROT];
	Json::Value scale = child[JSON_SCALE];
	glm::mat4 posMat4 = pos.size() > 0 ?
		glm::translate(glm::mat4(1.0f), glm::vec3(pos[0].asFloat(), pos[1].asFloat(), pos[2].asFloat()))
		: glm::mat4(0.0f);
	glm::mat4 rotMat4 = rot.size() > 0 ?
		glm::mat4_cast(glm::quat(rot[3].asFloat(), rot[0].asFloat(), rot[1].asFloat(), rot[2].asFloat())) : glm::mat4(1.0f);
	glm::mat4 scaleMat4 = scale.size() > 0 ?
		glm::scale(glm::mat4(1.0f), glm::vec3(scale[0].asFloat(), scale[1].asFloat(), scale[2].asFloat())) : glm::mat4(1.0f);
	if (!isMesh) {
		posMat4 = pos.size() > 0 ?
			glm::translate(glm::mat4(1.0f), glm::vec3(pos[0].asFloat() * 100.0, (pos[1].asFloat() +0.0) * 100.0, (pos[2].asFloat() + 0.0) * 100.0))
			: glm::mat4(0.0f);
	}
	if (isMesh) {
		scaleMat4 = scale.size() > 0 ?
			glm::scale(glm::mat4(1.0f), glm::vec3(scale[0].asFloat() * 100.0, scale[1].asFloat() * 100.0, scale[2].asFloat() * 100.0)) : glm::mat4(100.0f);
	}
	if (isNormal) {
		return rotMat4 * scaleMat4;
	}
	return  posMat4 * rotMat4 * scaleMat4;
}

bool FileParse::doFileParse() {
	Json::Value jsonRoot;
	Json::CharReaderBuilder readerBuilder;
	JSONCPP_STRING errs;
	std::ifstream ifs(fileName, std::ios::binary);
	if (!ifs.is_open()) {
		std::cerr << "open json file error." << std::endl;
		return false;
	}
	jsonRoot.clear();
	if (!parseFromStream(readerBuilder, ifs, &jsonRoot, &errs)) {
		std::cerr << "parse file stream error." << std::endl;
		return false;
	}
	Json::Value scenes = jsonRoot[JSON_SCENES];
	for (int i = 0; i < scenes.size(); i++) {
		auto scene = scenes[i];
		Json::Value objs = scene[JSON_OBJS];
		Json::Value meshes = scene[JSON_MESHES];
		glm::mat4 matrix;
		glm::mat4 normalMatrix;
		for (int q = 0; q < objs.size(); q++) {
			Model model;
			auto child = objs[q];
			matrix = createMatrix(child, false, false);
			normalMatrix = createMatrix(child, false, true);
			model = parseObjChild(child);
			model.matrix = matrix;
			model.normalMatrix = normalMatrix;
			models.push_back(model);
		}
		for (int p = 0; p < meshes.size(); p++) {
			Model model;
			auto child = meshes[p];
			if (child[JSON_MESH_NAME].asString() == "Ceiling" 
				|| child[JSON_MESH_NAME].asString() == "WallOuter") {
				continue;
			}
			matrix = createMatrix(child, true, false);
			normalMatrix = createMatrix(child, false, true);
			model = parseMeshChild(child);
			model.matrix = matrix;
			model.normalMatrix = normalMatrix;
			models.push_back(model);
		}
	}
	return true;
}

std::vector<Model> FileParse::getModels() const {
	return models;
}

void FileParse::setModels(const std::vector<Model>& _models) {
	models = _models;
}