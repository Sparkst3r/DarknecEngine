#include <SDL.h>
#include <render/shader/Shader.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class FontRenderer {
public:
	FontRenderer();
	FontRenderer(Shader shader);

	FontRenderer& bindFont(FT_Face font);
	FontRenderer& bindFont(std::string file);
	FontRenderer& setSize(float size);
	FontRenderer& setColour(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

	FT_Face getFont();

	void RenderFont(const char* text, float x, float y, float scaleX, float scaleY);

private:
	Shader shader;
	FT_Library freetype;
	FT_Face fontFace;
	GLuint vbo;

	float size = 18.0F;

	GLfloat red;
	GLfloat green;
	GLfloat blue;
	GLfloat alpha;

	GLint coordAttr;
	GLint texUnif;
	GLint colourUnif;
	
};


