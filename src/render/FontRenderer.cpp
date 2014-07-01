#include <render/FontRenderer.h>

struct Point {
	GLfloat xMin;
	GLfloat yMin;
	GLfloat xMax;
	GLfloat yMax;
};

FontRenderer::FontRenderer() {}

FontRenderer::FontRenderer(Shader shader) {
	this->shader = shader;
	if (FT_Init_FreeType(&this->freetype)) {
		SDL_LogError(3, "Could not initialize freetype library. Quitting.");
	}

	coordAttr = shader.getAttribute("coord");
	texUnif = shader.getUniform("tex");
	colourUnif = shader.getUniform("colour");
	glGenBuffers(1, &vbo);
}

FontRenderer& FontRenderer::bindFont(FT_Face font) {
	this->fontFace = font;
	return *this;
}

FontRenderer& FontRenderer::bindFont(std::string file) {
	if (FT_New_Face(this->freetype, file.c_str(), 0, &fontFace)) {
		std::string logText = "Could not find font: " + file;
		SDL_LogWarn(0,  logText.c_str());
	}
	return *this;
}

FontRenderer& FontRenderer::setSize(float size) {
	this->size = size;
	return *this;
}

FontRenderer& FontRenderer::setColour(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
	return *this;
}

FT_Face FontRenderer::getFont() {
	return this->fontFace;
}

void FontRenderer::RenderFont(const char* text, float x, float y, float scaleX, float scaleY) {
	shader.use();
	FT_Set_Pixel_Sizes(fontFace, 0, (FT_UInt) this->size);
	GLfloat colour[4]{this->red, this->green, this->blue, this->alpha};
	glUniform4fv(this->colourUnif, 1, colour);
	
	const char *p;
	FT_GlyphSlot g = fontFace->glyph;
	/* Create a texture that will be used to hold one "glyph" */
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(this->texUnif, 0);
	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/* Set up the VBO for our vertex data */
	glEnableVertexAttribArray(this->coordAttr);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(this->coordAttr, 4, GL_FLOAT, GL_FALSE, 0, 0);
	/* Loop through all characters */
	for (p = text; *p; p++) {
		/* Try to load and render the character */
		if (FT_Load_Char(fontFace, *p, FT_LOAD_RENDER))
			continue;
		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
		/* Calculate the vertex and texture coordinates */
		float x2 = x + g->bitmap_left * scaleX;
		float y2 = -y - g->bitmap_top * scaleY;
		float w = g->bitmap.width * scaleX;
		float h = g->bitmap.rows * scaleY;
		Point box[4] = {
			{x2, -y2, 0, 0},
			{x2 + w, -y2, 1, 0},
			{x2, -y2 - h, 0, 1},
			{x2 + w, -y2 - h, 1, 1},
		};
		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		/* Advance the cursor to the start of the next character */
		x += (g->advance.x >> 6) * scaleX;
		y += (g->advance.y >> 6) * scaleY;
	}
	glDisableVertexAttribArray(this->coordAttr);
	glDeleteTextures(1, &tex);
}