#ifndef DARKNEC_RENDERERFORWARDPHONG_H 
#define DARKNEC_RENDERERFORWARDPHONG_H

#include <State.h>
#include <render/Renderer.h>
#include <system/ShaderSystem.h>


class RendererForwardPhong : public Renderer {
public:
	RendererForwardPhong();

	void init();
	void render(Model model);
	void frameStepBefore();
	void frameStepAfter();

};

#endif // !DARKNEC_FORWARDPHONG_H
