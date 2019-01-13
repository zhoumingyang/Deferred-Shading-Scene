#include "gbuffer.h"

GeometryBuffer::GeometryBuffer() {
	frameBufferObject = new FBO();
	diffuseRbo = new RenderBuffer(GL_COLOR_ATTACHMENT0);
	positionRbo = new RenderBuffer(GL_COLOR_ATTACHMENT1);
	normalRbo = new RenderBuffer(GL_COLOR_ATTACHMENT2);
	depthRbo = new RenderBuffer(GL_DEPTH_ATTACHMENT);

	diffuseTex = new Texture();
	positionTex = new Texture();
	normalTex = new Texture();
}

GeometryBuffer::~GeometryBuffer() {
	if (frameBufferObject != NULL) {
		delete frameBufferObject;
		frameBufferObject = NULL;
	}
	if (diffuseRbo != NULL) {
		delete diffuseRbo;
		diffuseRbo = NULL;
	}
	if (positionRbo != NULL) {
		delete positionRbo;
		positionRbo = NULL;
	}
	if (normalRbo != NULL) {
		delete normalRbo;
		normalRbo = NULL;
	}
	if (diffuseTex != NULL) {
		delete diffuseTex;
		diffuseTex = NULL;
	}
	if (positionTex != NULL) {
		delete positionTex;
		positionTex = NULL;
	}
	if (normalTex != NULL) {
		delete normalTex;
		normalTex = NULL;
	}
}

void GeometryBuffer::initGbuffer() {
	const int m_width = frameBufferObject->getFrameWidthByIndex(0);
	const int m_height = frameBufferObject->getFrameHeightByIndex(0);
	frameBufferObject->bind(0);

	diffuseRbo->bind(0);
	diffuseRbo->renderBufferStorage(GL_RGBA, m_width, m_height);
	frameBufferObject->attachRenderBuffer(*diffuseRbo);

	positionRbo->bind(0);
	diffuseRbo->renderBufferStorage(GL_RGBA32F, m_width, m_height);
	frameBufferObject->attachRenderBuffer(*positionRbo);

	normalRbo->bind(0);
	normalRbo->renderBufferStorage(GL_RGBA16F, m_width, m_height);
	frameBufferObject->attachRenderBuffer(*normalRbo);

	depthRbo->bind(0);
	depthRbo->renderBufferStorage(GL_DEPTH_COMPONENT24, m_width, m_height);
	frameBufferObject->attachRenderBuffer(*depthRbo);

	diffuseTex->bind(0);
	diffuseTex->setTextureData(0, NULL, GL_RGBA, m_width, m_height, GL_RGBA);
	frameBufferObject->attachTexture(*diffuseTex, diffuseRbo->getAttachByIndex(0));
	
	positionTex->bind(0);
	positionTex->setTextureData(0, NULL, GL_RGBA32F, m_width, m_height, GL_RGBA);
	frameBufferObject->attachTexture(*positionTex, positionRbo->getAttachByIndex(0));

	normalTex->bind(0);
	normalTex->setTextureData(0, NULL, GL_RGBA16F, m_width, m_height, GL_RGBA);
	frameBufferObject->attachTexture(*normalTex, normalRbo->getAttachByIndex(0));

	frameBufferObject->frameBufferStatusCheck();
	frameBufferObject->unBind();
}

void GeometryBuffer::bind() const {
	frameBufferObject->bind(0);
}

void GeometryBuffer::unBind() const {
	frameBufferObject->unBind();
}

void GeometryBuffer::initDrawState() const {
	const int m_width = frameBufferObject->getFrameWidthByIndex(0);
	const int m_height = frameBufferObject->getFrameHeightByIndex(0);
	const GLenum diffuseAttachPoint = diffuseRbo->getAttachByIndex(0);
	const GLenum positionAttachPoint = positionRbo->getAttachByIndex(0);
	const GLenum normalAttachPoint = normalRbo->getAttachByIndex(0);

	bind();
	glPushAttrib(GL_VIEWPORT_BIT);
	//glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	GLenum buffers[] = { diffuseAttachPoint, positionAttachPoint, normalAttachPoint };
	glDrawBuffers(3, buffers);
}

void GeometryBuffer::releaseDrawState() const {
	unBind();
	glPopAttrib();
}

FBO GeometryBuffer::getFrameBufferObject() const {
	return *frameBufferObject;
}

RenderBuffer GeometryBuffer::getDiffuseRenderBuffer() const {
	return *diffuseRbo;
}

RenderBuffer GeometryBuffer::getPositionRenderBuffer() const {
	return *positionRbo;
}

RenderBuffer GeometryBuffer::getNormalRenderBuffer() const {
	return *normalRbo;
}

RenderBuffer GeometryBuffer::getDepthRenderBuffer() const {
	return *depthRbo;
}

Texture GeometryBuffer::getDiffuseTexture() const {
	return *diffuseTex;
}

Texture GeometryBuffer::getPositionTexture() const {
	return *positionTex;
}

Texture GeometryBuffer::getNormalTexture() const {
	return *normalTex;
}

void GeometryBuffer::activeDiffuseTexture(const GLuint& slot, const GLuint& index) const {
	diffuseTex->active(slot, index);
}

void GeometryBuffer::activePositionTexture(const GLuint& slot, const GLuint& index) const {
	positionTex->active(slot, index);
}

void GeometryBuffer::activeNormalTexture(const GLuint& slot, const GLuint& index) const {
	normalTex->active(slot, index);
}

void GeometryBuffer::deactiveDiffuseTexture(const GLuint& slot, const GLuint& index) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glDisable(GL_TEXTURE_2D);
	diffuseTex->unBind();
}

void GeometryBuffer::deactivePositionTexture(const GLuint& slot, const GLuint& index) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glDisable(GL_TEXTURE_2D);
	positionTex->unBind();
}

void GeometryBuffer::deactiveNormalTexture(const GLuint& slot, const GLuint& index) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glDisable(GL_TEXTURE_2D);
	normalTex->unBind();
}