#pragma once
#ifndef UI_H
#define UI_H
#include "camera.h"
class UI {
private:
	Camera camera;
public:
	UI(const Camera& _camera);
	~UI();
	void processMouseClickEvent(int button, int state, int x, int y);
	void processmouseMoveEvent(int x, int y);
	void processKeyDownEvent(unsigned char key, int x, int y);
};
#endif // !UI_H
