#include <render/gui/gui.h>

const float Gui::points[12] = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-1.0f, 1.0f,
	-1.0f, 1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f
};

Gui::Gui() {}
Gui::Gui(Shader shader) {
	this->shader = shader;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	// note: vertex buffer is already bound
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Gui::draw() {
	glDisable(GL_DEPTH_TEST);
	// absolute panel dimensions in pixels: 256x256px
	const float panel_width = 256.0f;
	const float panel_height = 256.0f;
	shader.use();
	// resize panel to size in pixels
	float x_scale = panel_width / 640;
	float y_scale = panel_height / 480;

	shader.setUniformFloat2("gui_scale", x_scale, y_scale);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}
