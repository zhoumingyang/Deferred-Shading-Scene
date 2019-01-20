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
#define MAXMESHNUMBER 1000
#define ORIDINARY_RENDER 1
#define DEFFERED_RENDER 2
#define BLOOM_RENDER 3
#define RENDER_CLEAR 4
#define RENDER_START 5

bool oridinaryRender = false;
bool defferedRender = true;
bool bloomRender = false;

const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 760;

//oridinary render
const char* pVSFileName = "./shader/showjson.vs";
const char* pFSFileName = "./shader/showjson.fs";
//deferred render
const char* pGvsFileName = "./shader/renderToGbuffer.vs";
const char* pGfsFileName = "./shader/renderToGbuffer.fs";
const char* pWvsFileName = "./shader/renderToWindow.vs";
const char* pWfsFileName = "./shader/renderToWindow.fs";
//bloom render
const char* pFvsFileName = "./shader/renderToFbuffer.vs";
const char* pFfsFileName = "./shader/renderToFbuffer.fs";
const char* pBlurvsFileName = "./shader/gaussianBlur.vs";
const char* pBlurfsFileName = "./shader/guassianBlur.fs";
const char* pFinalvsFileName = "./shader/renderFinal.vs";
const char* pFinalfsFileName = "./shader/renderFinal.fs";

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

/***************************uniform location********************************/
//oridinary render uniform location
GLuint mvpLocation;
GLuint worldMatLocation;
GLuint normalMatLocation;
GLuint texLocation;
GLuint specularLocation;
GLuint specularPowLocation;
GLuint eyeWorldPosLocation;
GLuint texMapLocation;
GLuint numPrleLightLocation;
GLuint numPointLightLocation;
GLuint numSpotLightLocation;
/***********************************************************/
//gBufers uniform location
GLuint gMvpLocation;
GLuint gWorldMatrixLocation;
GLuint gNormalMatrixLocation;
GLuint gTexLocation;
GLuint gIsMapLocation;
GLuint gSphereColorLocation;

//render to window uniform location
GLuint wDiffuseTexLocation;
GLuint wPositionTexLocation;
GLuint wNormalTexLocation;
GLuint wNumPrleLightLocation;
GLuint wNumPointLightLocation;
/***********************************************************/
//render to frame buffer uniform location
GLuint fDiffuseTexLocation;
GLuint fPositionTexLocation;
GLuint fNormalTexLocation;
GLuint fNumPrleLightLocation;
GLuint fNumPointLightLocation;

//blur render uniform location
GLuint bBrightImgLocation;
GLuint bHorizontal;

//render final to window
GLuint fnSceneImgLocation;
GLuint fnBlurImgLocation;
GLuint fnBloom;
GLuint fnExposure;

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

//quad mesh
Mesh* quaMesh;

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
		delete renderGBuffer;
		renderGBuffer = NULL;
	}
	if (renderWindow != NULL) {
		delete renderWindow;
		renderWindow = NULL;
	}
	if (renderFBuffer != NULL) {
		delete renderFBuffer;
		renderFBuffer = NULL;
	}
	if (renderBlur != NULL) {
		delete renderBlur;
		renderBlur = NULL;
	}
	if (renderFinal != NULL) {
		delete renderFinal;
		renderFinal = NULL;
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

	std::vector<ShapeGeometry*> lightSpheres;
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
}

void initUniformLocation() {
	if (renderOridinary) {
		mvpLocation = renderOridinary->getUniformLocation("mvp");
		worldMatLocation = renderOridinary->getUniformLocation("worldMatrix");
		normalMatLocation = renderOridinary->getUniformLocation("normalMatrix");
		texLocation = renderOridinary->getUniformLocation("tex");
		specularLocation = renderOridinary->getUniformLocation("specular");
		specularPowLocation = renderOridinary->getUniformLocation("specualrPower");
		eyeWorldPosLocation = renderOridinary->getUniformLocation("eyeWorldPos");
		texMapLocation = renderOridinary->getUniformLocation("texMap");
		numPrleLightLocation = renderOridinary->getUniformLocation("numPrleLight");
		numPointLightLocation = renderOridinary->getUniformLocation("numPointLight");
		numSpotLightLocation = renderOridinary->getUniformLocation("numSpotLight");
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
	if (renderGBuffer) {
		gMvpLocation = renderGBuffer->getUniformLocation("mvp");
		gWorldMatrixLocation = renderGBuffer->getUniformLocation("worldMatrix");
		gNormalMatrixLocation = renderGBuffer->getUniformLocation("normalMatrix");
		gTexLocation = renderGBuffer->getUniformLocation("tex");
		gIsMapLocation = renderGBuffer->getUniformLocation("isMap");
		gSphereColorLocation = renderGBuffer->getUniformLocation("lightSphereColor");
	}
	if (renderWindow) {
		wDiffuseTexLocation = renderWindow->getUniformLocation("tDiffuse");
		wPositionTexLocation = renderWindow->getUniformLocation("tPosition");
		wNormalTexLocation = renderWindow->getUniformLocation("tNormal");
		wNumPrleLightLocation = renderWindow->getUniformLocation("numPrleLight");
		wNumPointLightLocation = renderWindow->getUniformLocation("numPointLight");
		for (int i = 0; i < prleLights.size(); i++) {
			prleLights[i]->initUniformLocation(*renderWindow, i);
		}
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->initUniformLocation(*renderWindow, i);
		}
	}
	if (renderFBuffer) {
		fDiffuseTexLocation = renderFBuffer->getUniformLocation("tDiffuse");
		fPositionTexLocation = renderFBuffer->getUniformLocation("tPosition");
		fNormalTexLocation = renderFBuffer->getUniformLocation("tNormal");
		fNumPrleLightLocation = renderFBuffer->getUniformLocation("numPrleLight");
		fNumPointLightLocation = renderFBuffer->getUniformLocation("numPointLight");
		for (int i = 0; i < prleLights.size(); i++) {
			prleLights[i]->initUniformLocation(*renderFBuffer, i);
		}
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->initUniformLocation(*renderFBuffer, i);
		}
	}
	if (renderBlur) {
		bBrightImgLocation = renderBlur->getUniformLocation("brightImage");
		bHorizontal = renderBlur->getUniformLocation("horizontal");
	}
	if (renderFinal) {
		fnSceneImgLocation = renderFinal->getUniformLocation("sceneImage");
		fnBlurImgLocation = renderFinal->getUniformLocation("blurImage");
		fnBloom = renderFinal->getUniformLocation("bloom");
		fnExposure = renderFinal->getUniformLocation("exposure");
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
	diffuse = 5.0f;
	LightAttenuation attenuation(1.0f, 0.5f, 0.0f);
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			glm::vec3 tmpPosition = glm::vec3(-400 + drand48() * 1600, 60, -400 + drand48() * 800);
			lightColor = glm::vec3(drand48(), drand48(), drand48());
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
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
	FileParse* p = new FileParse("./design/scene-oneroom.json");
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

	renderFBO = new FBO();
	renderFBO->bind(0);
	colorTexture = new Texture(2);
	for (int i = 0; i < 2; i++) {
		colorTexture->bind(i);
		colorTexture->setTextureData(i, NULL, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT);
		renderFBO->attachTexture(*colorTexture, (GL_COLOR_ATTACHMENT0 + i), i);
	}
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	renderFBO->frameBufferStatusCheck();
	renderFBO->unBind();

	blurFBO = new FBO(2);
	blurTexture = new Texture(2);
	for (int i = 0; i < 2; i++) {
		blurFBO->bind(i);
		blurTexture->bind(i);
		blurTexture->setTextureData(i, NULL, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB);
		blurFBO->attachTexture(*blurTexture, GL_COLOR_ATTACHMENT0, i);
		blurFBO->frameBufferStatusCheck();
		blurTexture->unBind();
		blurFBO->unBind();
	}

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "some thing error: " << err << std::endl;
		clear();
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
			renderGBuffer->setUniform1i(gTexLocation, 0);
			renderGBuffer->setUniformMatrix4fv(gMvpLocation, &MVP[0][0]);
			renderGBuffer->setUniformMatrix4fv(gWorldMatrixLocation, &worldMat[0][0]);
			renderGBuffer->setUniformMatrix4fv(gNormalMatrixLocation, &normalMat[0][0]);
			renderGBuffer->setUniform3f(gSphereColorLocation, tmpColor);
			if (pMesh[i]->getMeshType() == NORMALMESH && pMesh[i]->getTexture() != NULL) {
				renderGBuffer->setUniform1i(gIsMapLocation, 1);
			}
			else if (pMesh[i]->getMeshType() == OBJMESH) {
				renderGBuffer->setUniform1i(gIsMapLocation, 0);
			}
			else if (pMesh[i]->getMeshType() == AIDMESH) {
				renderGBuffer->setUniform1i(gIsMapLocation, 2);
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
	renderFBuffer->setUniform1i(fNumPrleLightLocation, prleLights.size());
	renderFBuffer->setUniform1i(fNumPointLightLocation, pointLights.size());

	gBuffer->activeDiffuseTexture(0, 0);
	renderFBuffer->setUniform1i(fDiffuseTexLocation, 0);
	gBuffer->activePositionTexture(1, 0);
	renderFBuffer->setUniform1i(fPositionTexLocation, 1);
	gBuffer->activeNormalTexture(2, 0);
	renderFBuffer->setUniform1i(fNormalTexLocation, 2);
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
	renderWindow->setUniform1i(wNumPrleLightLocation, prleLights.size());
	renderWindow->setUniform1i(wNumPointLightLocation, pointLights.size());

	gBuffer->activeDiffuseTexture(0, 0);
	renderWindow->setUniform1i(wDiffuseTexLocation, 0);
	gBuffer->activePositionTexture(1, 0);
	renderWindow->setUniform1i(wPositionTexLocation, 1);
	gBuffer->activeNormalTexture(2, 0);
	renderWindow->setUniform1i(wNormalTexLocation, 2);
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
	renderOridinary->setUniform1f(specularLocation, 1.0);
	renderOridinary->setUniform1f(specularPowLocation, 1.0);
	renderOridinary->setUniform3f(eyeWorldPosLocation, eye);
	renderOridinary->setUniform1i(numPrleLightLocation, prleLights.size());
	renderOridinary->setUniform1i(numPointLightLocation, pointLights.size());
	renderOridinary->setUniform1i(numSpotLightLocation, spotLights.size());
	for (int i = 0; i < pMesh.size(); i++) {
		if (pMesh[i] != NULL) {
			glm::mat4 worldMat = pMesh[i]->getModelMatrix();
			glm::mat4 normalMat = pMesh[i]->getNormalMatrix();
			MV = camera.getModelViewMat(pMesh[i]->getModelMatrix());
			MVP = camera.getProjectViewModelMat(pMesh[i]->getModelMatrix());
			renderOridinary->setUniform1i(texLocation, 0);
			renderOridinary->setUniformMatrix4fv(mvpLocation, &MVP[0][0]);
			renderOridinary->setUniformMatrix4fv(worldMatLocation, &worldMat[0][0]);
			renderOridinary->setUniformMatrix4fv(normalMatLocation, &normalMat[0][0]);
			if (pMesh[i]->getMeshType() == NORMALMESH) {
				renderOridinary->setUniform1i(texMapLocation, 1);
			}
			else {
				renderOridinary->setUniform1i(texMapLocation, 0);
			}
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
		renderBlur->setUniform1i(bBrightImgLocation, 0);
		for (int i = 0; i < amount; i++) {
			blurFBO->bind(horizontal);
			renderBlur->setUniform1i(bHorizontal, horizontal);
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
		glEnable(GL_DEPTH_TEST);
		renderFinal->use();
		glActiveTexture(GL_TEXTURE0);
		colorTexture->bind(0);
		renderFinal->setUniform1i(fnSceneImgLocation, 0);
		glActiveTexture(GL_TEXTURE1);
		blurTexture->bind(!horizontal);
		renderFinal->setUniform1i(fnBlurImgLocation, 1);
		renderFinal->setUniform1i(fnBloom, true);
		renderFinal->setUniform1f(fnExposure, 1.0);
		quaMesh->render();
		blurTexture->unBind();
		colorTexture->unBind();
		renderFinal->unuse();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//draw standar line
	//initOptionDisable();
	//renderStandarLine();
	glutSwapBuffers();
	glutPostRedisplay();
}

void processMouseClickEvent(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if ((x >= 0 && x < WINDOW_WIDTH) && (y >= 0 && y < WINDOW_HEIGHT)) {
			mouseDown = true;
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
	switch (option) {
	case ORIDINARY_RENDER:
		oridinaryRender = true;
		defferedRender = false;
		bloomRender = false;
		break;
	case DEFFERED_RENDER:
		oridinaryRender = false;
		defferedRender = true;
		bloomRender = false;
		break;
	case BLOOM_RENDER:
		oridinaryRender = false;
		defferedRender = false;
		bloomRender = true;
		break;
	case RENDER_CLEAR:
		oridinaryRender = false;
		defferedRender = false;
		bloomRender = false;
		clear();
		break;
	case RENDER_START:
		oridinaryRender = false;
		defferedRender = true;
		bloomRender = false;
		clear();
		initGL();
		break;
	default:
		oridinaryRender = false;
		defferedRender = true;
		bloomRender = false;
	}
	glutPostRedisplay();
}

void createRightMenu() {
	int menu;
	menu = glutCreateMenu(processRightMenuEvents);
	glutAddMenuEntry("forward rendering", ORIDINARY_RENDER);
	glutAddMenuEntry("deffered rendering", DEFFERED_RENDER);
	glutAddMenuEntry("bloom rendering", BLOOM_RENDER);
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