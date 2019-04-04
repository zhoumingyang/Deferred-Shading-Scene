#include <windows.h>
#include <ctime>
#include "fileparse.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "parallellight.h"
#include "pointlight.h"
#include "spotlight.h"
#include "drand.h"
#include "gbuffer.h"
#include "shapegeometry.h"
#include "sphere.h"
#include "rectangle.h"
#include "texturecube.h"
#include "ray.h"
#include "box3math.h"
#include "spheremath.h"
#include "trianglemath.h"
#define MAXMESHNUMBER 1000
#define ORIDINARY_RENDER 1
#define DEFFERED_RENDER 2
#define BLOOM_RENDER 3
#define RENDER_CLEAR 4
#define RENDER_START 5
#define SHADOW_RENDER 6

bool oridinaryRender = true;
bool defferedRender = false;
bool bloomRender = false;
bool shadowRender = false;

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;

const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

//oridinary render
const char* pVSFileName = "./shader/showjson.vert";
const char* pFSFileName = "./shader/showjson.frag";
//deferred render
const char* pGvsFileName = "./shader/renderToGbuffer.vert";
const char* pGfsFileName = "./shader/renderToGbuffer.frag";
const char* pWvsFileName = "./shader/renderToWindow.vert";
const char* pWfsFileName = "./shader/renderToWindow.frag";
//bloom render
const char* pFvsFileName = "./shader/renderToFbuffer.vert";
const char* pFfsFileName = "./shader/renderToFbuffer.frag";
const char* pBlurvsFileName = "./shader/gaussianBlur.vert";
const char* pBlurfsFileName = "./shader/guassianBlur.frag";
const char* pFinalvsFileName = "./shader/renderFinal.vert";
const char* pFinalfsFileName = "./shader/renderFinal.frag";
//shadow depth
const char* pSdvsFileName = "./shader/shadowDepth.vert";
const char* pSdgsFileName = "./shader/shadowDepth.geom";
const char* pSdfsFileName = "./shader/shadowDepth.frag";
//shadow render
const char* pSvsFileName = "./shader/renderShadow.vert";
const char* pSfsFileName = "./shader/renderShadow.frag";
//select effect
const char* pSevsFileName = "./shader/selectEffect.vert";
const char* pSefsFileName = "./shader/selectEffect.frag";

std::vector<Model> models;
/***************************camera********************************/
GLfloat fov = 60.0;
GLfloat aspect = WINDOW_WIDTH / WINDOW_HEIGHT;
GLfloat znear = 1.0;
GLfloat zfar = 10000;
glm::vec3 eye = glm::vec3(0, 60, 200);
glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
Camera camera;

/*************************matrix**********************************/
glm::mat4 molMat4;
glm::mat4 MVP;
glm::mat4 MV;

/*************************shader**********************************/
Shader* renderOridinary;
Shader* renderGBuffer;
Shader* renderWindow;
Shader* renderFBuffer;
Shader* renderBlur;
Shader* renderFinal;
Shader* shadowDepth;
Shader* renderShadow;
Shader* renderSelected;

/*************************all lights**********************************/
glm::vec3 spotLightPosition(0.0, 100.0, 0.0);
std::vector<Parallellight*> prleLights;
std::vector<PointLight*> pointLights;
std::vector<SpotLight*> spotLights;
std::vector<ShapeGeometry*> lightSpheres;

/*************************mouse operation**********************************/
bool mouseDown = false;
bool firstMouse = true;
float lastx = WINDOW_WIDTH / 2.0f;
float lasty = WINDOW_HEIGHT / 2.0f;
float deltaTime = 0.0f;
clock_t lastFrame = 0.0;

GeometryBuffer* gBuffer;
FBO* renderFBO;
Texture* colorTexture;
FBO* blurFBO;
Texture* blurTexture;
FBO *shadowDepthFBO;
Texture* shadowDepthTexture;

//quad mesh
Mesh* quaMesh;

float nearPlane = 1.0f;
float farPlane = 10000.0f;//150.0f;

//select mesh
MeshUnit selectedMeshUnit;
Mesh* selectedMesh;
bool selectedFlag = false;

void renderStandarLine() {
	glLoadIdentity();
	glPushMatrix();
	gluPerspective(glm::radians(fov), aspect, znear, zfar);
	gluLookAt(eye.x, eye.y, eye.z, center.x, center.y, center.z, up.x, up.y, up.z);
	//x line
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();

	//y line
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glEnd();

	//z line
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glEnd();

	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 1.0f, 0.6f);
	glVertex3f(eye.x, eye.y, eye.z);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.6f, 0.6f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(eye.x, eye.y, eye.z);
	glEnd();

	glPopMatrix();
}

void clear() {
	std::vector <Model>().swap(models);
	if (renderOridinary != NULL) {
		std::vector<Mesh*> pMesh = renderOridinary->getMeshes();
		for (int i = 0; i < pMesh.size(); i++) {
			if (pMesh[i] != NULL) {
				delete pMesh[i];
				pMesh[i] = NULL;
			}
		}
		std::vector<Mesh*>().swap(pMesh);
		delete renderOridinary;
		renderOridinary = NULL;
	}
	if (renderGBuffer != NULL) {
		std::vector<Mesh*> pMesh = renderGBuffer->getMeshes();
		std::vector<Mesh*>().swap(pMesh);
		delete renderGBuffer;
		renderGBuffer = NULL;
	}
	if (renderWindow != NULL) {
		std::vector<Mesh*> pMesh = renderWindow->getMeshes();
		std::vector<Mesh*>().swap(pMesh);
		delete renderWindow;
		renderWindow = NULL;
	}
	if (renderFBuffer != NULL) {
		std::vector<Mesh*> pMesh = renderFBuffer->getMeshes();
		std::vector<Mesh*>().swap(pMesh);
		delete renderFBuffer;
		renderFBuffer = NULL;
	}
	if (renderBlur != NULL) {
		std::vector<Mesh*> pMesh = renderBlur->getMeshes();
		std::vector<Mesh*>().swap(pMesh);
		delete renderBlur;
		renderBlur = NULL;
	}
	if (renderFinal != NULL) {
		std::vector<Mesh*> pMesh = renderFinal->getMeshes();
		std::vector<Mesh*>().swap(pMesh);
		delete renderFinal;
		renderFinal = NULL;
	}
	if (shadowDepth) {
		std::vector<Mesh*> pMesh = shadowDepth->getMeshes();
		std::vector<Mesh*>().swap(pMesh);
		delete shadowDepth;
		shadowDepth = NULL;
	}
	if (renderShadow) {
		std::vector<Mesh*> pMesh = renderShadow->getMeshes();
		std::vector<Mesh*>().swap(pMesh);
		delete renderShadow;
		renderShadow = NULL;
	}

	for (int i = 0; i < prleLights.size(); i++) {
		if (prleLights[i] != NULL) {
			delete prleLights[i];
			prleLights[i] = NULL;
		}
	}

	for (int i = 0; i < pointLights.size(); i++) {
		if (pointLights[i] != NULL) {
			delete pointLights[i];
			pointLights[i] = NULL;
		}
	}

	for (int i = 0; i < spotLights.size(); i++) {
		if (spotLights[i] != NULL) {
			delete spotLights[i];
			spotLights[i] = NULL;
		}
	}
	std::vector<Parallellight*>().swap(prleLights);
	std::vector<PointLight*>().swap(pointLights);
	std::vector<SpotLight*>().swap(spotLights);

	for (int i = 0; i < lightSpheres.size(); i++) {
		if (lightSpheres[i] != NULL) {
			delete lightSpheres[i];
			lightSpheres[i] = NULL;
		}
	}
	std::vector<ShapeGeometry*>().swap(lightSpheres);
	if (gBuffer != NULL) {
		delete gBuffer;
		gBuffer = NULL;
	}
	if (quaMesh != NULL) {
		delete quaMesh;
		quaMesh = NULL;
	}
	if (renderFBO != NULL) {
		delete renderFBO;
		renderFBO = NULL;
	}
	if (colorTexture != NULL) {
		delete colorTexture;
		colorTexture = NULL;
	}
	if (blurFBO != NULL) {
		delete blurFBO;
		blurFBO = NULL;
	}
	if (blurTexture != NULL) {
		delete blurTexture;
		blurTexture = NULL;
	}
	if (shadowDepthFBO != NULL) {
		delete shadowDepthFBO;
		shadowDepthFBO = NULL;
	}
	if (shadowDepthTexture != NULL) {
		delete shadowDepthTexture;
		shadowDepthTexture = NULL;
	}
}

void testShowMultiModelGL(FileParse* p) {
	models = p->getModels();
	int modelCount = models.size();
	int sphereCount = lightSpheres.size();
	std::vector<Mesh*> pMesh;

	for (int i = 0; i < sphereCount; i++) {
		Mesh* pm = new Mesh(*lightSpheres[i]);
		pm->initBuffer();
		pm->initTexture();
		pMesh.push_back(pm);
	}

	for (int i = 0; i < modelCount; i++) {
		Model currentModel = models[i];
		if (currentModel.isMesh) {
			Mesh* pM = new Mesh(currentModel);
			pM->initBuffer();
			pM->initTexture();
			pMesh.push_back(pM);
		}
		else {
			Mesh* pO = new Mesh(currentModel);
			pO->initBuffer();
			pO->initTexture();
			pMesh.push_back(pO);
		}
	}
	renderOridinary->setMeshes(pMesh);
	renderGBuffer->setMeshes(pMesh);
	shadowDepth->setMeshes(pMesh);
	renderShadow->setMeshes(pMesh);
}

void initUniformLocation() {
	if (renderOridinary) {
		for (int i = 0; i < prleLights.size(); i++) {
			prleLights[i]->initUniformLocation(*renderOridinary, i);
		}
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->initUniformLocation(*renderOridinary, i);
		}
		for (int i = 0; i < spotLights.size(); i++) {
			spotLights[i]->initUniformLocation(*renderOridinary, i);
		}
	}
	if (renderWindow) {
		for (int i = 0; i < prleLights.size(); i++) {
			prleLights[i]->initUniformLocation(*renderWindow, i);
		}
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->initUniformLocation(*renderWindow, i);
		}
	}
	if (renderFBuffer) {
		for (int i = 0; i < prleLights.size(); i++) {
			prleLights[i]->initUniformLocation(*renderFBuffer, i);
		}
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->initUniformLocation(*renderFBuffer, i);
		}
	}
	if (renderShadow) {
		//only consider point light
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->initUniformLocation(*renderShadow, i);
		}
	}
}

void initLight() {
	//direction light
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambient = 0.75f;
	float diffuse = 1.0f;
	glm::vec3 lightDirection(-0.0f, -1.0f, -0.0f);
	prleLights.push_back(new Parallellight(lightColor, ambient, diffuse, lightDirection));

	//point light
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	diffuse = shadowRender ? 50.0f : 6.0f;
	LightAttenuation attenuation(1.0f, 0.5f, 0.0f);
	int lightNumber = shadowRender ? 1 : 6;       //now in shadow mode we render just one point light
	for (int i = 0; i < lightNumber; i++) {
		for (int j = 0; j < lightNumber; j++) {
			glm::vec3 tmpPosition;
			if (shadowRender) {
				tmpPosition = glm::vec3(drand48() * 30, 60, drand48() * 30);
				lightColor = glm::vec3(1.0, 1.0, 1.0);
			}
			else {
				tmpPosition = glm::vec3(-400 + drand48() * 1600, 60, -400 + drand48() * 800);
				//lightColor = glm::vec3(drand48(), drand48(), drand48());
				lightColor = glm::vec3(1.0, 1.0, 1.0);
			}
			PointLight* pLight = new PointLight(tmpPosition, attenuation, lightColor, ambient, diffuse);
			pointLights.push_back(pLight);
			ShapeGeometry* tmpShape = new Sphere();
			tmpShape->setPosition(tmpPosition);
			tmpShape->setColor(lightColor);
			lightSpheres.push_back(tmpShape);
		}
	}

	//spot light
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	lightDirection = glm::vec3(0.0f, -1.0f, 0.0f);
	float cutoff = 0.75f;
	diffuse = 1.0f;
	LightAttenuation spAttenuation(1.0f, 0.25f, 0.0f);
	spotLights.push_back(new SpotLight(spotLightPosition, lightDirection, cutoff, spAttenuation, lightColor, ambient, diffuse));
}

void initCamera() {
	Perspective per = Perspective(fov, aspect, znear, zfar);
	glm::vec3 ce = center - eye;
	glm::vec3 tmpUp = glm::cross(ce, glm::vec3(-1, 0, 0));
	View view = View(eye);
	camera = Camera(per, view);
}

void initOptionEnable() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);

	GLint iMultiSample = 0;
	GLint iNumSamples = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
	glGetIntegerv(GL_SAMPLES, &iNumSamples);
	//printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);
}

void initOptionDisable() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);
	glDisable(GL_POLYGON_SMOOTH);
}

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//shader init
	//oridinary
	renderOridinary = new Shader(pVSFileName, pFSFileName);
	renderOridinary->compileShader();
	//deferred
	renderGBuffer = new Shader(pGvsFileName, pGfsFileName);
	renderGBuffer->compileShader();
	renderWindow = new Shader(pWvsFileName, pWfsFileName);
	renderWindow->compileShader();
	//bloom
	renderFBuffer = new Shader(pFvsFileName, pFfsFileName);
	renderFBuffer->compileShader();
	renderBlur = new Shader(pBlurvsFileName, pBlurfsFileName);
	renderBlur->compileShader();
	renderFinal = new Shader(pFinalvsFileName, pFinalfsFileName);
	renderFinal->compileShader();
	//shadow depth
	shadowDepth = new Shader(pSdvsFileName, pSdfsFileName, pSdgsFileName);
	shadowDepth->compileShader();
	//shadow render
	renderShadow = new Shader(pSvsFileName, pSfsFileName);
	renderShadow->compileShader();
	//selected render
	renderSelected = new Shader(pSevsFileName, pSefsFileName);
	renderSelected->compileShader();

	//scene init
	initLight();
	initCamera();

	//quad mesh init
	ShapeGeometry *rect = new Rect();
	quaMesh = new Mesh(*rect);
	quaMesh->initBuffer();

	//shader uniform var init 
	initUniformLocation();

	//init vao & vbo & texture
	FileParse* p = new FileParse("./design/scene-intersect.json");
	if (!p->doFileParse()) {
		delete p;
		return;
	}
	testShowMultiModelGL(p);

	//init geometry buffer
	gBuffer = new GeometryBuffer();
	if (gBuffer == NULL) {
		std::cout << "gen geometry buffer error" << std::endl;
		clear();
		exit(1);
	}
	gBuffer->initGbuffer();

	//for bloom render - render to bright image and diffuse image in frame buffer
	renderFBO = new FBO();
	renderFBO->bind(0);
	colorTexture = new Texture2D(2);
	for (int i = 0; i < 2; i++) {
		colorTexture->bind(i);
		colorTexture->setTextureData(i, NULL, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT);
		renderFBO->attachTexture(*colorTexture, (GL_COLOR_ATTACHMENT0 + i), i);
	}
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	renderFBO->frameBufferStatusCheck();
	renderFBO->unBind();

	//for bloom render - blur bright image use gaussian blur
	blurFBO = new FBO(2);
	blurTexture = new Texture2D(2);
	for (int i = 0; i < 2; i++) {
		blurFBO->bind(i);
		blurTexture->bind(i);
		blurTexture->setTextureData(i, NULL, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB);
		blurFBO->attachTexture(*blurTexture, GL_COLOR_ATTACHMENT0, i);
		blurFBO->frameBufferStatusCheck();
		blurTexture->unBind();
		blurFBO->unBind();
	}

	//shadow render - shadow depth texture attach to a frame buffer
	shadowDepthFBO = new FBO();
	shadowDepthFBO->bind(0);
	shadowDepthTexture = new TextureCube();
	shadowDepthTexture->bind(0);
	for (int i = 0; i < 6; i++) {
		shadowDepthTexture->setTextureData(i, NULL, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT);
	}
	shadowDepthTexture->setFilterMode(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	shadowDepthTexture->setFilterMode(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	shadowDepthTexture->setWrapMode(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	shadowDepthTexture->setWrapMode(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	shadowDepthTexture->setWrapMode(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	shadowDepthFBO->attachTexture(*shadowDepthTexture, GL_DEPTH_ATTACHMENT, 0, false);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	shadowDepthFBO->frameBufferStatusCheck();
	shadowDepthFBO->unBind();

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "some thing error: " << err << std::endl;
		clear();
		getchar();
		exit(1);
	}
	delete p;
}

void multiRenderGBuffer(const Camera& camera) {
	std::vector<Mesh*> pMesh = renderGBuffer->getMeshes();
	for (int i = 0; i < pMesh.size(); i++) {
		if (pMesh[i] != NULL) {
			glm::mat4 worldMat = pMesh[i]->getModelMatrix();
			glm::mat4 normalMat = pMesh[i]->getNormalMatrix();
			glm::vec3 tmpColor = pMesh[i]->getColor();
			MVP = camera.getProjectViewModelMat(pMesh[i]->getModelMatrix());
			renderGBuffer->setUniform1i("tex", 0);
			renderGBuffer->setUniformMatrix4fv("mvp", &MVP[0][0]);
			renderGBuffer->setUniformMatrix4fv("worldMatrix", &worldMat[0][0]);
			renderGBuffer->setUniformMatrix4fv("normalMatrix", &normalMat[0][0]);
			renderGBuffer->setUniform3f("lightSphereColor", tmpColor);
			if (pMesh[i]->getMeshType() == NORMALMESH && pMesh[i]->getTexture() != NULL) {
				renderGBuffer->setUniform1i("isMap", 1);
			}
			else if (pMesh[i]->getMeshType() == OBJMESH) {
				renderGBuffer->setUniform1i("isMap", 0);
			}
			else if (pMesh[i]->getMeshType() == AIDMESH) {
				renderGBuffer->setUniform1i("isMap", 2);
			}
			pMesh[i]->render();
		}
	}
}

void multiRenderFBuffer() {
	if (prleLights.size() > 0) {
		for (int i = 0; i < prleLights.size(); i++) {
			prleLights[i]->setAllUniformParams(*renderFBuffer);
		}
	}
	if (pointLights.size() > 0) {
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->setAllUniformParams(*renderFBuffer);
		}
	}
	renderFBuffer->setUniform1i("numPrleLight", prleLights.size());
	renderFBuffer->setUniform1i("numPointLight", pointLights.size());

	gBuffer->activeDiffuseTexture(0, 0);
	renderFBuffer->setUniform1i("tDiffuse", 0);
	gBuffer->activePositionTexture(1, 0);
	renderFBuffer->setUniform1i("tPosition", 1);
	gBuffer->activeNormalTexture(2, 0);
	renderFBuffer->setUniform1i("tNormal", 2);
	quaMesh->render();
	gBuffer->deactiveDiffuseTexture(0, 0);
	gBuffer->deactivePositionTexture(1, 0);
	gBuffer->deactiveNormalTexture(2, 0);
}

void multiRenderWindow() {
	if (prleLights.size() > 0) {
		for (int i = 0; i < prleLights.size(); i++) {
			prleLights[i]->setAllUniformParams(*renderWindow);
		}
	}
	if (pointLights.size() > 0) {
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->setAllUniformParams(*renderWindow);
		}
	}
	renderWindow->setUniform1i("numPrleLight", prleLights.size());
	renderWindow->setUniform1i("numPointLight", pointLights.size());

	gBuffer->activeDiffuseTexture(0, 0);
	renderWindow->setUniform1i("tDiffuse", 0);
	gBuffer->activePositionTexture(1, 0);
	renderWindow->setUniform1i("tPosition", 1);
	gBuffer->activeNormalTexture(2, 0);
	renderWindow->setUniform1i("tNormal", 2);
	quaMesh->render();
	gBuffer->deactiveDiffuseTexture(0, 0);
	gBuffer->deactivePositionTexture(1, 0);
	gBuffer->deactiveNormalTexture(2, 0);
}

void multiRenderOridinary(const Camera& camera) {
	std::vector<Mesh*> pMesh = renderOridinary->getMeshes();
	glm::mat4 _proMat = camera.getProjectMat();
	glm::mat4 _viewMat = camera.getViewMat();
	if (prleLights.size() > 0) {
		for (int i = 0; i < prleLights.size(); i++) {
			prleLights[i]->setAllUniformParams(*renderOridinary);
		}
	}
	if (pointLights.size() > 0) {
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->setAllUniformParams(*renderOridinary);
		}
	}
	if (spotLights.size() > 0) {
		for (int i = 0; i < spotLights.size(); i++) {
			spotLights[i]->setAllUniformParams(*renderOridinary);
		}
	}
	renderOridinary->setUniform1f("specular", 1.0);
	renderOridinary->setUniform1f("specualrPower", 1.0);
	renderOridinary->setUniform3f("eyeWorldPos", eye);
	renderOridinary->setUniform1i("numPrleLight", prleLights.size());
	renderOridinary->setUniform1i("numPointLight", pointLights.size());
	renderOridinary->setUniform1i("numSpotLight", spotLights.size());
	for (int i = 0; i < pMesh.size(); i++) {
		if (pMesh[i] != NULL) {
			glm::mat4 worldMat = pMesh[i]->getModelMatrix();
			glm::mat4 normalMat = pMesh[i]->getNormalMatrix();
			MV = camera.getModelViewMat(pMesh[i]->getModelMatrix());
			MVP = camera.getProjectViewModelMat(pMesh[i]->getModelMatrix());
			renderOridinary->setUniform1i("tex", 0);
			renderOridinary->setUniformMatrix4fv("mvp", &MVP[0][0]);
			renderOridinary->setUniformMatrix4fv("worldMatrix", &worldMat[0][0]);
			renderOridinary->setUniformMatrix4fv("normalMatrix", &normalMat[0][0]);
			if (pMesh[i]->getMeshType() == NORMALMESH) {
				renderOridinary->setUniform1i("texMap", 1);
			}
			else {
				renderOridinary->setUniform1i("texMap", 0);
			}
			pMesh[i]->render();
		}
	}
}

void mutiRenderShadowDepth(std::vector<glm::mat4> lightSpaceMatrix) {
	for (int i = 0; i < 6; i++) {
		const std::string varName = "shadowMatrices[" + std::to_string(i) + "]";
		shadowDepth->setUniformMatrix4fv(varName, &lightSpaceMatrix[i][0][0]);
	}
	glm::vec3 lightPos = pointLights[0]->getPosition();
	shadowDepth->setUniform3f("lightPos", lightPos);
	shadowDepth->setUniform1f("far_plane", farPlane);
	std::vector<Mesh*> pMesh = shadowDepth->getMeshes();
	for (int i = 0; i < pMesh.size(); i++) {
		if (pMesh[i] != NULL) {
			glm::mat4 worldMat = pMesh[i]->getModelMatrix();
			shadowDepth->setUniformMatrix4fv("worldMatrix", &worldMat[0][0]);
			pMesh[i]->render();
		}
	}
}

void mutiRenderShadowWindow() {
	std::vector<Mesh*> pMesh = renderShadow->getMeshes();
	if (pointLights.size() > 0) {
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->setAllUniformParams(*renderShadow);
		}
	}
	renderShadow->setUniform1f("farPlane", farPlane);
	renderShadow->setUniform1f("specular", 1.0);
	renderShadow->setUniform1f("specualrPower", 1.0);
	renderShadow->setUniform3f("eyeWorldPos", eye);
	renderShadow->setUniform1i("numPointLight", pointLights.size());
	renderShadow->setUniform1i("shadowMap", 1);
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_CUBE_MAP);
	shadowDepthTexture->bind(0);
	int aidCount = 0;
	for (int i = 0; i < pMesh.size(); i++) {
		if (pMesh[i] != NULL) {
			glm::mat4 worldMat = pMesh[i]->getModelMatrix();
			glm::mat4 normalMat = pMesh[i]->getNormalMatrix();
			MV = camera.getModelViewMat(pMesh[i]->getModelMatrix());
			MVP = camera.getProjectViewModelMat(pMesh[i]->getModelMatrix());
			renderShadow->setUniform1i("tex", 0);
			renderShadow->setUniform1i("depthMap", 1);
			renderShadow->setUniformMatrix4fv("mvp", &MVP[0][0]);
			renderShadow->setUniformMatrix4fv("worldMatrix", &worldMat[0][0]);
			renderShadow->setUniformMatrix4fv("normalMatrix", &normalMat[0][0]);
			if (pMesh[i]->getMeshType() == NORMALMESH) {
				renderShadow->setUniform1i("texMap", 1);
			}
			else {
				renderShadow->setUniform1i("texMap", 0);
			}
			pMesh[i]->render();
		}
	}
	glDisable(GL_TEXTURE_CUBE_MAP);
	shadowDepthTexture->unBind();
}

void renderSelectedMesh() {
	std::vector<Mesh*> pMesh = renderSelected->getMeshes();
	renderSelected->setUniform3f("pointColor", glm::vec3(0.32, 0.32, 0.85));
	renderSelected->setUniform1f("opacity", 0.65);
	for (int i = 0; i < pMesh.size(); i++) {
		if (pMesh[i] != NULL) {
			glm::mat4 worldMat = pMesh[i]->getModelMatrix();
			glm::mat4 normalMat = pMesh[i]->getNormalMatrix();
			MV = camera.getModelViewMat(pMesh[i]->getModelMatrix());
			MVP = camera.getProjectViewModelMat(pMesh[i]->getModelMatrix());
			renderSelected->setUniformMatrix4fv("mvp", &MVP[0][0]);
			pMesh[i]->render();
		}
	}
}

void renderGL() {
	clock_t currentFrame = clock();
	deltaTime = float(currentFrame - lastFrame) / 1000.0;
	lastFrame = currentFrame;
	initOptionEnable();

	if (defferedRender) {
		//render to texture
		if (renderGBuffer == NULL || gBuffer == NULL || renderGBuffer == NULL) {
			return;
		}
		renderGBuffer->use();
		gBuffer->initDrawState();
		multiRenderGBuffer(camera);
		gBuffer->releaseDrawState();
		renderGBuffer->unuse();

		//deferred render to window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		renderWindow->use();
		multiRenderWindow();
		renderWindow->unuse();
	}
	else if (oridinaryRender) {
		//oridinary render
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (renderOridinary == NULL) {
			return;
		}
		renderOridinary->use();
		multiRenderOridinary(camera);
		renderOridinary->unuse();
	}
	else if (bloomRender) {
		if (renderGBuffer == NULL || gBuffer == NULL || renderGBuffer == NULL) {
			return;
		}
		renderGBuffer->use();
		gBuffer->initDrawState();
		multiRenderGBuffer(camera);
		gBuffer->releaseDrawState();
		renderGBuffer->unuse();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderFBuffer->use();
		renderFBO->bind(0);
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
		multiRenderFBuffer();
		renderFBO->unBind();
		renderFBuffer->unuse();

		bool horizontal = true, first = true;
		unsigned int amount = 4;
		renderBlur->use();
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		renderBlur->setUniform1i("brightImage", 0);
		for (int i = 0; i < amount; i++) {
			blurFBO->bind(horizontal);
			renderBlur->setUniform1i("horizontal", horizontal);
			first ? colorTexture->bind(1) : blurTexture->bind(!horizontal);
			quaMesh->render();
			horizontal = !horizontal;
			if (first) { first = false; }
		}
		colorTexture->unBind();
		blurTexture->unBind();
		blurFBO->unBind();
		renderBlur->unuse();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderFinal->use();
		glActiveTexture(GL_TEXTURE0);
		colorTexture->bind(0);
		renderFinal->setUniform1i("sceneImage", 0);
		glActiveTexture(GL_TEXTURE1);
		blurTexture->bind(!horizontal);
		renderFinal->setUniform1i("blurImage", 1);
		renderFinal->setUniform1i("bloom", true);
		renderFinal->setUniform1f("exposure", 1.0);
		quaMesh->render();
		blurTexture->unBind();
		colorTexture->unBind();
		renderFinal->unuse();
	}
	else if (shadowRender) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 lightProj = glm::perspective(glm::radians(60.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, nearPlane, farPlane);
		std::vector<glm::mat4> lightSpaceMatrix;
		for (int i = 0; i < pointLights.size(); i++) {
			PointLight* ptl = pointLights[i];
			glm::vec3 lightPos = ptl->getPosition();
			lightSpaceMatrix.push_back(lightProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			lightSpaceMatrix.push_back(lightProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			lightSpaceMatrix.push_back(lightProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			lightSpaceMatrix.push_back(lightProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
			lightSpaceMatrix.push_back(lightProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
			lightSpaceMatrix.push_back(lightProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		}
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		shadowDepthFBO->bind(0);
		shadowDepthFBO->frameBufferStatusCheck();
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowDepth->use();
		glCullFace(GL_FRONT);
		mutiRenderShadowDepth(lightSpaceMatrix);
		glCullFace(GL_BACK);
		shadowDepth->unuse();
		shadowDepthFBO->unBind();

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderShadow->use();
		mutiRenderShadowWindow();
		renderShadow->unuse();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	initOptionDisable();
	if (selectedFlag == true && oridinaryRender) {
		renderSelected->use();
		renderSelectedMesh();
		renderSelected->unuse();
	}

	//draw standar line
	//initOptionDisable();
	renderStandarLine();
	glutSwapBuffers();
	glutPostRedisplay();
}

bool doPickObject(int x, int y) {
	selectedFlag = false;

	//viewport space to ndc space
	float nx = (2.0f * float(x)) / float(WINDOW_WIDTH) - 1.0f;
	float ny = 1.0f - (2.0f * float(y)) / float(WINDOW_HEIGHT);
	float nz = 1.0f;
	glm::vec3 ver_ndc = glm::vec3(nx, ny, nz);

	//ndc space to clip space
	glm::vec4 ver_clip = glm::vec4(ver_ndc.x, ver_ndc.y, -1.0, 1.0);

	//clip space to eye space
	glm::mat4 projectMat4 = camera.getProjectMat();
	glm::mat4 inverseProjectMat4 = glm::inverse(projectMat4);
	glm::vec4 ver_eye = inverseProjectMat4 * ver_clip;
	ver_eye = glm::vec4(ver_eye.x, ver_eye.y, -1.0, 0.0);

	//eye space to world space
	glm::mat4 viewMat4 = camera.getViewMat();
	glm::mat4 inverseViewMat4 = glm::inverse(viewMat4);
	glm::vec4 ver_world = inverseViewMat4 * ver_eye;
	ver_world = glm::normalize(ver_world);
	glm::vec3 rayDir = glm::vec3(ver_world.x, ver_world.y, ver_world.z);

	//test intersect
	glm::vec3 rayOrigin = camera.getView().eye;
	Ray ray = Ray(rayOrigin, rayDir);
	std::vector<Mesh*> pMesh = renderGBuffer->getMeshes();
	if (pMesh.size() > 0) {
		SphereMath sphereBounding;
		Box3Math box3Bounding;
		std::vector<MeshUnit> candidateSelected;
		std::vector<float> distances;
		std::vector<glm::mat4> worldMats;
		std::vector<glm::mat4> normalMats;
		for (int i = 0; i < pMesh.size(); i++) {
			MeshType meshType = pMesh[i]->getMeshType();
			if (meshType == OBJMESH) {
				std::vector<MeshUnit> meshDatas = pMesh[i]->getMeshUnits();
				//world space to model space
				glm::mat4 worldMat = pMesh[i]->getModelMatrix();
				glm::mat4 normalMat = pMesh[i]->getNormalMatrix();
				Ray tmpRay = Ray(rayOrigin, rayDir);
				glm::mat4 inverseModelMat4 = glm::inverse(worldMat);
				tmpRay.applyMat4(inverseModelMat4);

				for (int j = 0; j < meshDatas.size(); j++) {
					MeshUnit meshData = meshDatas[j];
					sphereBounding.setFromPosition(meshData.vertices);
					box3Bounding.setFromPosition(meshData.vertices);
					bool sphereIntersect = tmpRay.intersectsSphere(sphereBounding);
					bool boxIntersect = tmpRay.intersectsBox(box3Bounding);
					if (sphereIntersect && boxIntersect) {
						std::vector<int> indices = meshData.indices;
						for (int k = 0; k + 2 < indices.size(); k += 3) {
							int v1 = indices[k];
							int v2 = indices[k + 1];
							int v3 = indices[k + 2];

							glm::vec3 vA = meshData.vertices[v1];
							glm::vec3 vB = meshData.vertices[v2];
							glm::vec3 vC = meshData.vertices[v3];

							glm::vec3 intersectPoint = tmpRay.intersectTriangle(vA, vB, vC, false);
							if (intersectPoint.x != NULL && intersectPoint.y != NULL && intersectPoint.z != NULL) {
								selectedFlag = true;
								worldMats.push_back(worldMat);
								normalMats.push_back(normalMat);
								candidateSelected.push_back(meshData);
								float tmpDistance = glm::distance(rayOrigin, intersectPoint);
								distances.push_back(tmpDistance);
							}
						}
					}
				}
			}
		}
		if (!selectedFlag) {
			return false;
		}
		int minDisIndex = 0;
		float minDis = 0.0;
		for (int i = 0; i < distances.size(); i++) {
			if (distances[i] > minDis) {
				minDis = distances[i];
				minDisIndex = i;
			}
		}
		selectedMeshUnit = candidateSelected[minDisIndex];
		glm::mat4 tmpWorldMat = worldMats[minDisIndex];
		glm::mat4 tmpNormalMat = normalMats[minDisIndex];
		selectedMesh = new Mesh(selectedMeshUnit, tmpWorldMat, tmpNormalMat, SELECTED);
		selectedMesh->initBuffer();
		std::vector<Mesh*> selectedMeshes = { selectedMesh };
		renderSelected->setMeshes(selectedMeshes);
	}
	return true;
}

void processMouseClickEvent(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if ((x >= 0 && x < WINDOW_WIDTH) && (y >= 0 && y < WINDOW_HEIGHT)) {
			mouseDown = true;
			doPickObject(x, y);
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mouseDown = false;
		firstMouse = true;
	}
	else if (button == GLUT_MIDDLE_BUTTON) {

	}
}

void processmouseMoveEvent(int x, int y) {
	if (mouseDown) {
		if (firstMouse) {
			lastx = float(x);
			lasty = float(y);
			firstMouse = false;
		}
		float xoffset = x - lastx;
		float yoffset = lasty - y;
		lastx = float(x);
		lasty = float(y);
		xoffset = -xoffset;
		yoffset = -yoffset;
		camera.processMouseMovement(xoffset, yoffset, false);
	}
}

void processKeyDownEvent(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		clear();
		exit(1);
		break;
	case 'w':
		camera.processKeyBoard(FORWARD, deltaTime);
		break;
	case 's':
		camera.processKeyBoard(BACKWARD, deltaTime);
		break;
	case 'a':
		camera.processKeyBoard(LEFT, deltaTime);
		break;
	case 'd':
		camera.processKeyBoard(RIGHT, deltaTime);
		break;
	case 'q':
		camera.processKeyBoard(UP, deltaTime);
		break;
	case 'e':
		camera.processKeyBoard(DOWN, deltaTime);
		break;
	}
}

void processRightMenuEvents(int option) {
	if (shadowRender &&
		(option == ORIDINARY_RENDER || option == DEFFERED_RENDER || option == BLOOM_RENDER)) {
		clear();
	}

	switch (option) {
	case ORIDINARY_RENDER:
		oridinaryRender = true;
		defferedRender = false;
		bloomRender = false;
		if (shadowRender) {
			shadowRender = false;
			initGL();
		}
		break;
	case DEFFERED_RENDER:
		oridinaryRender = false;
		defferedRender = true;
		bloomRender = false;
		if (shadowRender) {
			shadowRender = false;
			initGL();
		}
		break;
	case BLOOM_RENDER:
		oridinaryRender = false;
		defferedRender = false;
		bloomRender = true;
		if (shadowRender) {
			shadowRender = false;
			initGL();
		}
		break;
	case SHADOW_RENDER:
		oridinaryRender = false;
		defferedRender = false;
		bloomRender = false;
		shadowRender = true;
		clear();
		initGL();
		break;
	case RENDER_CLEAR:
		oridinaryRender = false;
		defferedRender = false;
		bloomRender = false;
		shadowRender = false;
		clear();
		break;
	case RENDER_START:
		oridinaryRender = false;
		defferedRender = true;
		bloomRender = false;
		shadowRender = false;
		clear();
		initGL();
		break;
	default:
		oridinaryRender = false;
		defferedRender = true;
		bloomRender = false;
		shadowRender = false;
	}
	glutPostRedisplay();
}

void createRightMenu() {
	int menu;
	menu = glutCreateMenu(processRightMenuEvents);
	glutAddMenuEntry("forward rendering", ORIDINARY_RENDER);
	glutAddMenuEntry("deffered rendering", DEFFERED_RENDER);
	glutAddMenuEntry("bloom rendering", BLOOM_RENDER);
	glutAddMenuEntry("shadow rendering", SHADOW_RENDER);
	glutAddMenuEntry("rendering clear", RENDER_CLEAR);
	glutAddMenuEntry("rendering start", RENDER_START);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(200, 0);
	glutCreateWindow("json show");
	glutDisplayFunc(renderGL);
	glutIdleFunc(renderGL);
	glutMouseFunc(processMouseClickEvent);
	glutMotionFunc(processmouseMoveEvent);
	glutKeyboardFunc(processKeyDownEvent);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		getchar();
		return 1;
	}
	initGL();
	createRightMenu();
	glutMainLoop();
	clear();
	getchar();
	return 0;
}