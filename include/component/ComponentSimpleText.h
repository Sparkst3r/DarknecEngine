#ifndef DARKNEC_COMPONENTSIMPLETEXT_H
#define DARKNEC_COMPONENTSIMPLETEXT_H

#include <State.h>
#include <component/Component.h>
#include <component/ComponentTransform.h>
#include <component/ComponentRWUtils.h>

#include <render/shader/Shader.h>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	float xAdvance;
	float yAdvance;
	float left;
	float top;
	float width;
	float height;

	float xTexOffset;
};

class FontAtlas {
public:
	FBO atlasFBO_;
	GLuint atlasTexture_;
	std::vector<Character> characters_ = std::vector<Character>(128);
	int atlasWidth_;
	int atlasHeight_;
};


class ComponentSimpleText : public Component {
public:


	DECL_COMPONENT_FACTORY(ComponentSimpleText)
	DECL_COMPONENTREQUIREMENT_FACTORY(ComponentSimpleText)

	ComponentSimpleText(GameObject* container);

	virtual void update();
	virtual void read(XMLNode node);
	virtual void init();

	virtual bool validate();

	virtual XMLNode write(XMLNode node);


	FontAtlas generateAtlas();

	void ComponentSimpleText::RenderFont(const char* text, float scaleX, float scaleY);

	void Render();

	void bindFont(std::string file);

	void drawSquare();

	FT_Face getFont();

	void setSize(float size);
	void setColour(glm::vec4 colour);
	void setShader(Shader shader);
	void setText(std::string text);

private:
	ComponentRequirement<ComponentTransform> transform_;

	FontAtlas atlas_;

	FT_Library freetype_;
	FT_Face fontface_;
	float fontsize_;
	glm::vec4 fontcolour_;

	float textWidth;
	float textHeight;

	float textOldWidth = 0.0;
	float textOldHeight = 0.0;
	
	std::string text_;
	GLuint textTexture_;

	VBO vbo_;
	IBO ibo_;
	VAO vao_;


	VBO vbo2_;
	IBO ibo2_;
	VAO vao2_;

	FBO fbo_;
	GLuint tex1_;



};
#endif // !DARKNEC_COMPONENTSIMPLETEXT_H