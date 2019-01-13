#pragma once
#ifndef G_BUFFER
#define G_BUFFER
#include "fbo.h"
#include "renderbuffer.h"
#include "texture.h"

class GeometryBuffer {
private:
	FBO *frameBufferObject;
	RenderBuffer *diffuseRbo;
	RenderBuffer *positionRbo;
	RenderBuffer *normalRbo;
	RenderBuffer *depthRbo;
	Texture *diffuseTex;
	Texture *positionTex;
	Texture *normalTex;
public:
	GeometryBuffer();
	~GeometryBuffer();
	void initGbuffer();
	void bind() const;
	void unBind() const;
	void initDrawState() const;
	void releaseDrawState() const;
	FBO getFrameBufferObject() const;
	RenderBuffer getDiffuseRenderBuffer() const;
	RenderBuffer getPositionRenderBuffer() const;
	RenderBuffer getNormalRenderBuffer() const;
	RenderBuffer getDepthRenderBuffer() const;
	Texture getDiffuseTexture() const;
	Texture getPositionTexture() const;
	Texture getNormalTexture() const;
	void activeDiffuseTexture(const GLuint& slot, const GLuint& index) const;
	void activePositionTexture(const GLuint& slot, const GLuint& index) const;
	void activeNormalTexture(const GLuint& slot, const GLuint& index) const;
	void deactiveDiffuseTexture(const GLuint& slot, const GLuint& index) const;
	void deactivePositionTexture(const GLuint& slot, const GLuint& index) const;
	void deactiveNormalTexture(const GLuint& slot, const GLuint& index) const;
};

#endif // !G_BUFFER
