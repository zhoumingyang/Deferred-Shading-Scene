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
#define MAXMESHNUMBER 1000
const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 760;

const char* pVSFileName = "./shader/showjson.vs";
const char* pFSFileName = "./shader/showjson.fs";
const char* pGvsFileName = "./shader/renderToGbuffer.vs";
const char* pGfsFileName = "./shader/renderToGbuffer.fs";
const char* pWvsFileName = "./shader/renderToWindow.vs";
const char* pWfsFileName = "./shader/renderToWindow.fs";

std::vector<Model> models;

GLfloat fov = 60.0;
GLfloat aspect = WINDOW_WIDTH / WINDOW_HEIGHT;
GLfloat znear = 1.0;
GLfloat zfar = 10000;
glm::vec3 eye = glm::vec3(0, 60, 200);
glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
glm::vec3 center = glm::vec3(0.0, 0.0, 0.0);
Camera camera;

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

GLuint gMvpLocation;
GLuint gWorldMatrixLocation;
GLuint gNormalMatrixLocation;
GLuint gTexLocation;
GLuint gIsMapLocation;

GLuint wDiffuseTexLocation;
GLuint wPositionTexLocation;
GLuint wNormalTexLocation;
GLuint wNumPrleLightLocation;
GLuint wNumPointLightLocation;

glm::mat4 molMat4;
glm::mat4 MVP;
glm::mat4 MV;

Shader* renderOridinary;
Shader* renderGBuffer;
Shader* renderWindow;
glm::vec3 pointLightPosition(0.0, 40.0, 0.0);
glm::vec3 spotLightPosition(0.0, 100.0, 0.0);
std::vector<Parallellight*> prleLights;
std::vector<PointLight*> pointLights;
std::vector<SpotLight*> spotLights;

std::vector<Parallellight*> wPrleLights;
std::vector<PointLight*> wPointLights;

bool mouseDown = false;
bool firstMouse = true;
float lastx = WINDOW_WIDTH / 2.0f;
float lasty = WINDOW_HEIGHT / 2.0f;
float deltaTime = 0.0f;
clock_t lastFrame = 0.0;

GeometryBuffer* gBuffer;

//quad mesh
Mesh* quaMesh;
std::vector<glm::vec3> vertices = {
	glm::vec3(-0.98, -0.98, 0.0f),
	glm::vec3(0.98, -0.98, 0.0f),
	glm::vec3(0.98, 0.98, 0.0f),
	glm::vec3(-0.98, 0.98, 0.0f),
};

std::vector<glm::vec2> uvs = {
	glm::vec2(0.0f, 0.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(1.0f, 1.0f),
	glm::vec2(0.0f, 1.0f)
};

std::vector<glm::vec3> normals = {
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 0.0f, 1.0f)
};

std::vector<int> indices = {
	0, 1, 2,
	2, 3, 0
};

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

	glPointSize(10.0);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.5f, 1.0f);
	glVertex3f(pointLightPosition.x / 10.0, pointLightPosition.y / 10.0, pointLightPosition.z / 10.0);
	glEnd();

	glPointSize(10.0);
	glBegin(GL_POINTS);
	glColor3f(0.0f, 0.5f, 1.0f);
	glVertex3f(spotLightPosition.x / 10.0, spotLightPosition.y / 10.0, spotLightPosition.z / 10.0);
	glEnd();

	glPopMatrix();
}

void testShowMultiModelGL(FileParse* p) {
	models = p->getModels();
	int modelCount = models.size();
	std::vector<Mesh*> pMesh;
	pMesh.resize(modelCount);
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
	}
	if (renderWindow) {
		wDiffuseTexLocation = renderWindow->getUniformLocation("tDiffuse");
		wPositionTexLocation = renderWindow->getUniformLocation("tPosition");
		wNormalTexLocation = renderWindow->getUniformLocation("tNormal");
		wNumPrleLightLocation = renderWindow->getUniformLocation("numPrleLight");
		wNumPointLightLocation = renderWindow->getUniformLocation("numPointLight");
		for (int i = 0; i < wPrleLights.size(); i++) {
			if (wPrleLights[i] != NULL) {
				wPrleLights[i]->initUniformLocation(*renderWindow, i);
			}
		}
		for (int i = 0; i < wPointLights.size(); i++) {
			if (wPointLights[i] != NULL) {
				wPointLights[i]->initUniformLocation(*renderWindow, i);
			}
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
	wPrleLights.push_back(new Parallellight(lightColor, ambient, diffuse, lightDirection));

	//point light
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	diffuse = 15.0f;
	LightAttenuation attenuation(1.0f, 0.5f, 0.0f);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			glm::vec3 tmpPosition = glm::vec3(-200 + drand48() * 400, 80, -200 + drand48() * 400);
			lightColor = glm::vec3(drand48(), drand48(), drand48());
			PointLight* pLight = new PointLight(tmpPosition, attenuation, lightColor, ambient, diffuse);
			pointLights.push_back(pLight);
			wPointLights.push_back(pLight);
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
	renderOridinary = new Shader(pVSFileName, pFSFileName);
	renderOridinary->compileShader();
	renderGBuffer = new Shader(pGvsFileName, pGfsFileName);
	renderGBuffer->compileShader();
	renderWindow = new Shader(pWvsFileName, pWfsFileName);
	renderWindow->compileShader();

	//scene init
	initLight();
	initCamera();

	//quad mesh init
	MeshUnit meshUnit(vertices, uvs, normals, indices);
	quaMesh = new Mesh(meshUnit);
	quaMesh->initBuffer();

	//shader uniform var init 
	initUniformLocation();

	//init vao & vbo
	FileParse* p = new FileParse("./design/scene-oneroom.json");
	if (!p->doFileParse()) {
		delete p;
		return;
	}
	testShowMultiModelGL(p);

	gBuffer = new GeometryBuffer();
	if (gBuffer == NULL) {
		std::cout << "gen geometry buffer error" << std::endl;
		delete gBuffer;
		delete p;
		delete renderOridinary;
		delete renderGBuffer;
		exit(1);
	}
	gBuffer->initGbuffer();

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cout << "some thing error: " << err << std::endl;
		delete gBuffer;
		delete p;
		delete renderOridinary;
		delete renderGBuffer;
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
			MVP = camera.getProjectViewModelMat(pMesh[i]->getModelMatrix());
			renderGBuffer->setUniform1i(gTexLocation, 0);
			renderGBuffer->setUniformMatrix4fv(gMvpLocation, &MVP[0][0]);
			renderGBuffer->setUniformMatrix4fv(gWorldMatrixLocation, &worldMat[0][0]);
			renderGBuffer->setUniformMatrix4fv(gNormalMatrixLocation, &normalMat[0][0]);
			if (pMesh[i]->getMeshType() == NORMALMESH && pMesh[i]->getTexture() != NULL) {
				renderGBuffer->setUniform1i(gIsMapLocation, 1);
			}
			else {
				renderGBuffer->setUniform1i(gIsMapLocation, 0);
			}
			pMesh[i]->render();
		}
	}
}

void multiRenderWindow() {
	if (wPrleLights.size() > 0) {
		for (int i = 0; i < wPrleLights.size(); i++) {
			wPrleLights[i]->setAllUniformParams();
		}
	}
	if (wPointLights.size() > 0) {
		for (int i = 0; i < wPointLights.size(); i++) {
			wPointLights[i]->setAllUniformParams();
		}
	}
	renderWindow->setUniform1i(wNumPrleLightLocation, wPrleLights.size());
	renderWindow->setUniform1i(wNumPointLightLocation, wPointLights.size());

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
			prleLights[i]->setAllUniformParams();
		}
	}
	if (pointLights.size() > 0) {
		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i]->setAllUniformParams();
		}
	}
	if (spotLights.size() > 0) {
		for (int i = 0; i < spotLights.size(); i++) {
			spotLights[i]->setAllUniformParams();
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

	//render to texture
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

	//oridinary render
	//renderOridinary->use();
	//multiRenderOridinary(camera);
	//renderOridinary->unuse();

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
	glutMainLoop();
	getchar();
	return 0;
}