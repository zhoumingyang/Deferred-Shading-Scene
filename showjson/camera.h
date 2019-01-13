#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include "glm.hpp"

#define YAW -90.0
#define PITCH 0.0
#define SPEED 500.5
#define SENSITIVITY 0.1
#define ZOOM 60.0

enum CAMERA_MOVEMENT {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Perspective {
	float fov;
	float aspect;
	float znear;
	float zfar;
	Perspective(float _fov, float _aspect, float _znear, float _zfar) {
		fov = _fov;
		aspect = _aspect;
		znear = _znear;
		zfar = _zfar;
	}
	Perspective() {
		fov = 0;
		aspect = 0;
		znear = 0;
		zfar = 0;
	}
};

struct View {
	glm::vec3 eye;
	glm::vec3 up;
	glm::vec3 worldUp;
	glm::vec3 right;
	glm::vec3 front;
	View(const glm::vec3& _eye = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& _up = glm::vec3(0.0f, 1.0f, 0.0f)) {
		eye = _eye;
		worldUp = _up;
		front = glm::vec3(0.0f - eye.x, 0.0f - eye.y, 0.0f - eye.z);
	}
};

class Camera{
private:
	View view;
	Perspective perspective;
	float yaw;
	float pitch;
	float movementSpeed;
	float mouseSensitivity;
	float zoom;
	void updateCameraVectors();
public:
	Camera();
	Camera(const Perspective& _perspective, const View& _view, const float& _yaw = YAW, const float& _pitch = PITCH);
	~Camera();
	Perspective getPerspective() const;
	View getView() const;
	void setPerspective(const Perspective& _perspective);
	void setView(const View& _view);
	void processKeyBoard(CAMERA_MOVEMENT direction, const float& deltaTime);
	void processMouseMovement(float& xoffset, float& yoffset, const bool& constrainPitch = true);
	void processMouseScroll(const float& yoffset);
	glm::mat4 getProjectMat() const;
	glm::mat4 getViewMat() const;
	glm::mat4 getProjectViewMat() const;
	glm::mat4 getProjectViewModelMat(const glm::mat4& modelMat) const;
	glm::mat4 getModelViewMat(const glm::mat4& modelMat) const;
};

#endif // !CAMERA_H