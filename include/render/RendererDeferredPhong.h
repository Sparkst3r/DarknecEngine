#ifndef DARKNEC_RENDERERDEFERREDPHONG_H 
#define DARKNEC_RENDERERDEFERREDPHONG_H

#include <State.h>
#include <render/Renderer.h>
#include <system/ShaderSystem.h>
#include <render/Texture.h>

class GBuffer {
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();

	~GBuffer();

private:

	GLuint m_fbo;
	Texture m_textures[GBUFFER_NUM_TEXTURES];
	Texture m_depthTexture;
};

class RendererDeferredPhong : public Renderer {
public:

	void render(Model model);

};

#endif // !DARKNEC_RENDERERDEFERREDPHONG_H
