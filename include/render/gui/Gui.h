#ifndef DARKNEC_GUI_H
#define DARKNEC_GUI_H

#include <render/GLTypeDef.h>
#include <render/shader/Shader.h>
#include <stdio.h>



class Gui {
public:

	Gui();
	Gui(Shader shader);

	void draw();

	static const float points[12];

private:
	VAO vao;
	VBO vbo;
	Shader shader;
};


#endif