#include "camera.h"

Camera::Camera(const Perspective& _perspective, const View& _view, const float& _yaw, const float& _pitch) {
	perspective = _perspective;
	view = _view;
	yaw = _yaw;
	pitch = _pitch;
	movementSpeed = SPEED;
	mouseSensitivity = SENSITIVITY;
	zoom = ZOOM;
	updateCameraVectors();
}

Camera::Camera() {

}

Camera::~Camera() {

}

void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	view.front = glm::normalize(front);
	view.right = glm::normalize(glm::cross(view.front, view.worldUp));
	view.up = glm::normalize(glm::cross(view.right, view.front));
}

Perspective Camera::getPerspective() const {
	return perspective;
}

View Camera::getView() const {
	return view;
}

void Camera::setPerspective(const Perspective& _perspective) {
	perspective = _perspective;
}

void Camera::setView(const View& _view) {
	view = _view;
}

glm::mat4 Camera::getProjectMat() const {
	return glm::perspective(glm::radians(perspective.fov), perspective.aspect, perspective.znear, perspective.zfar);
}

glm::mat4 Camera::getViewMat() const {
	return  glm::lookAt(view.eye, view.eye + view.front, view.up);
}

glm::mat4 Camera::getProjectViewMat() const {
	glm::mat4 proMat4 = getProjectMat();
	glm::mat4 viewMat4 = getViewMat();
	return proMat4 * viewMat4;
}

glm::mat4 Camera::getProjectViewModelMat(const glm::mat4& modelMat) const {
	glm::mat4 proMat4 = getProjectMat();
	glm::mat4 viewMat4 = getViewMat();
	return proMat4 * viewMat4 * modelMat;
}

glm::mat4 Camera::getModelViewMat(const glm::mat4& modelMat) const {
	glm::mat4 viewMat4 = getViewMat();
	return viewMat4 * modelMat;
}

void Camera::processKeyBoard(CAMERA_MOVEMENT direction, const float& deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD) {
		view.eye += view.front * velocity;
	}
	else if (direction == BACKWARD) {
		view.eye -= view.front * velocity;
	}
	else if (direction == LEFT) {
		view.eye -= view.right * velocity;
	}
	else if (direction == RIGHT) {
		view.eye += view.right * velocity;
	}
	else if (direction == UP) {
		view.eye += view.up * velocity;
	}
	else if (direction == DOWN) {
		view.eye -= view.up * velocity;
	}
}

void Camera::processMouseMovement(float& xoffset, float& yoffset, const bool& constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::processMouseScroll(const float& yoffset) {
	if (zoom >= 1.0f && zoom <= 90.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}