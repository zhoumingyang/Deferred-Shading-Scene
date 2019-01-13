#include "fbo.h"

FBO::FBO() {
	count = 1;
	fbos = new GLuint[count];
	widths.push_back(1024);
	heights.push_back(760);
	glGenFramebuffers(count, fbos);
}

FBO::FBO(const std::vector<float>& _widths, const std::vector<float>& _heights, const int& _count) {
	int withCount = _widths.size();
	int heightCount = _heights.size();
	int tmpCount = withCount > _count ? _count : withCount;
	count = tmpCount > heightCount ? heightCount : tmpCount;
	if (count <= 0) {
		count = 1;
		widths.push_back(1024);
		heights.push_back(760);
	}
	else {
		for (int i = 0; i < count; i++) {
			widths.push_back(_widths[i]);
			heights.push_back(_heights[i]);
		}
	}
	fbos = new GLuint[count];
	glGenFramebuffers(count, fbos);
}

FBO::FBO(float _widths[], float _heights[], const int& _count) {
	if (count <= 0) {
		count = 1;
		widths.push_back(1024);
		heights.push_back(760);
	}
	else {
		for (int i = 0; i < count;i++) {
			widths.push_back(_widths[i]);
			heights.push_back(_heights[i]);
		}
	}
	fbos = new GLuint[count];
	glGenFramebuffers(count, fbos);
}

FBO::FBO(const float& _width, const float& _height) {
	count = 1;
	fbos = new GLuint[count];
	widths.push_back(_width);
	heights.push_back(_height);
	glGenFramebuffers(count, fbos);
}

FBO::~FBO() {
	glDeleteFramebuffers(count, fbos);
	if (fbos) {
		delete[] fbos;
		fbos = NULL;
	}
	count = 0;
}

void FBO::bind(const int& index) const {
	glBindFramebuffer(GL_FRAMEBUFFER, fbos[index]);
}

void FBO::unBind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint* FBO::getFrameObjects() const {
	return fbos;
}

GLuint FBO::getFrameObjectByIndex(const int& index) const {
	return fbos[index];
}

int FBO::getFrameObjectsCount() const {
	return count;
}

std::vector<float> FBO::getAllHeights() const {
	return heights;
}

std::vector<float> FBO::getAllWidths() const {
	return widths;
}

float FBO::getFrameHeightByIndex(const int& index) const {
	return heights[index];
}

float  FBO::getFrameWidthByIndex(const int& index) const {
	return widths[index];
}

void FBO::attachRenderBuffer(const RenderBuffer& renderBuffer) const {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, renderBuffer.getAttachByIndex(0), 
		GL_RENDERBUFFER, renderBuffer.getRenderObjectByIndex(0));
}

void FBO::attachTexture(const Texture& texture, const GLenum& attachPoint) const {
	texture.setOridinaryTexParam();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachPoint, GL_TEXTURE_2D, texture.getTexObjectByIndex(0), 0);
}

void FBO::frameBufferStatusCheck() const {
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");
}