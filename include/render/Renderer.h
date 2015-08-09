#ifndef DARKNEC_RENDERER_H 
#define DARKNEC_RENDERER_H

#include <object/Model.h>

class Renderer {
public:

	virtual void render(Model model) = 0;


};

#endif // !DARKNEC_RENDERER_H
