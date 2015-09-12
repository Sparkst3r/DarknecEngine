#ifndef DARKNEC_RENDERERDEFERREDPHONG_H 
#define DARKNEC_RENDERERDEFERREDPHONG_H

#include <State.h>
#include <render/Renderer.h>
#include <system/ShaderSystem.h>
#include <render/Texture.h>

class GBuffer {
public:
	GBuffer();
	GLuint m_fbo;
	GLuint m_rbo;
	void destroy();



	std::vector<Texture> textures = std::vector<Texture>(4);
};

class RendererDeferredPhong : public Renderer {
public:
	RendererDeferredPhong();
	void frameStepBefore();
	void frameStepAfter();
	GBuffer* buffer = NULL;

	void render(Model model);
	void geometryPass(GBuffer*, Model);
	


	void init();



	GLuint VBO;
	GLuint IBO;
	GLuint VAO;
};

#endif // !DARKNEC_RENDERERDEFERREDPHONG_H
