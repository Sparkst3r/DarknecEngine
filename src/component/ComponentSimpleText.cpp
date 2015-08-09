#include <component/ComponentSimpleText.h>
#include <system/ShaderSystem.h>


ComponentSimpleText::ComponentSimpleText(GameObject* container) {
	container_ = container;
	if (FT_Init_FreeType(&this->freetype_)) {
		Darknec::logger(Darknec::LogLevel::LOG_ERROR, "Could not initialize freetype library.");
	}
	fontsize_ = 20.0;




}

bool ComponentSimpleText::validate() {
	return this->transform_.validate() && this->transform_->validate();
}

void ComponentSimpleText::bindFont(std::string file) {
	if (FT_New_Face(this->freetype_, file.c_str(), 0, &this->fontface_)) {
		Darknec::logger(Darknec::LogLevel::LOG_WARN, "Could not find font : %s", file.c_str());
	}
	else {
		Darknec::logger(Darknec::LogLevel::LOG_WARN, "Found font : %s", file.c_str());
		this->atlas_ = generateAtlas();
	}
}

void ComponentSimpleText::setSize(float size) {
	this->fontsize_ = size;
}



void ComponentSimpleText::setColour(glm::vec4 colour) {
	this->fontcolour_ = colour;
}

FT_Face ComponentSimpleText::getFont() {
	return this->fontface_;
}


static const unsigned int indices[] {
	0, 1, 2, 0, 3, 1,
};

//
//
//void ComponentSimpleText::RenderFont(const char* text, float scaleX, float scaleY) {
//
//
//	int x = 0;
//	int y = 0;
//
//	if (this->shader_.isValid()) {
//		shader_.use();
//	}
//	else {
//		shader_ = Shader::getActiveShader();
//	}
//	
//	
//	
//	FT_Set_Pixel_Sizes(this->fontface_, 0, (FT_UInt) this->fontsize_);
//	shader_.setUniformFloatVec4("fontColour", this->fontcolour_);
//
//	const char *p;
//	FT_GlyphSlot g = this->fontface_->glyph;
//
//	/* Create a texture that will be used to hold one "glyph" */
//	GLuint tex;
//	glActiveTexture(GL_TEXTURE1);
//	glGenTextures(1, &tex);
//	glBindTexture(GL_TEXTURE_2D, tex);
//
//	shader_.setUniformInt("fontTexture", 1);
//
//	/* We require 1 byte alignment when uploading texture data */
//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//	/* Clamping to edges is important to prevent artifacts when scaling */
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	/* Linear filtering usually looks best for text */
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//
//
//
//
//
//	/* Loop through all characters */
//	for (p = text; *p; p++) {
//		/* Try to load and render the character */
//		if (FT_Load_Char(this->fontface_, *p, FT_LOAD_RENDER)) {
//			continue;
//			Darknec::logger("Hi");
//		}
//
//
//		/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
//		/* Calculate the vertex and texture coordinates */
//		
//
//
//
//
//
//		float x2 = x  + g->bitmap_left * scaleX;
//		float y2 = -y - g->bitmap_top * scaleY;
//		float w = g->bitmap.width * scaleX;
//		float h = g->bitmap.rows * scaleY;
//
//
//		float data[16] = {
//			//Positions        //UVS
//			x2, -y2,           0.0, 0.0,
//			x2 + w, -y2 - h,   1.0, 1.0,
//			x2 + w, -y2,       1.0, 0.0,
//			x2, -y2 - h,       0.0, 1.0,
//			
//		};
//
//
//
//		glGenBuffers(1, &this->ibo_);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
//
//		/* Set up the VBO for our vertex data */
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
//		glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), data, GL_DYNAMIC_DRAW);
//		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);
//
//
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//
//
//
//		/* Advance the cursor to the start of the next character */
//		x += (g->advance.x >> 6) * scaleX;
//		y += (g->advance.y >> 6) * scaleY;
//	}
//	glDisableVertexAttribArray(0);
//	glDeleteTextures(1, &tex);
//	glActiveTexture(GL_TEXTURE0);
//}
//


Shader copyshader;
FontAtlas ComponentSimpleText::generateAtlas() {
	FontAtlas atlas = FontAtlas();
	sys3->useShader("Pass");
	int w = 0;
	int h = 0;
	int x = 0;

	glActiveTexture(GL_TEXTURE3);
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



	FT_Set_Pixel_Sizes(this->fontface_, 0, (FT_UInt) 100);
	FT_GlyphSlot glyph = this->fontface_->glyph;

#pragma region Load Atlas size
	for (int c = 32; c < atlas.characters_.size(); c++) { //Skip cont-codes
		if (FT_Load_Char(this->fontface_, c, FT_LOAD_RENDER)) {
			Darknec::logger("Loading character %c failed!", c);
			continue;
		}
		if (glyph->bitmap.width < 0.000001f) {
			w += glyph->advance.x >> 6;
		}
		else {
			w += glyph->bitmap.width;
		}

		h = std::max(h, glyph->bitmap.rows);
	}
	atlas.atlasWidth_ = w;
	atlas.atlasHeight_ = h;
#pragma endregion


#pragma region FBO

	glGenFramebuffers(1, &this->fbo_);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbo_);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, atlas.atlasWidth_);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, atlas.atlasHeight_);
	glFramebufferParameteri(GL_DRAW_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_SAMPLES, 4);

	glActiveTexture(GL_TEXTURE4);
	glGenTextures(1, &tex1_);
	glBindTexture(GL_TEXTURE_2D, tex1_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas.atlasWidth_, atlas.atlasHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	std::vector<GLubyte> bytes = std::vector<GLubyte>(atlas.atlasWidth_ * atlas.atlasHeight_ * 4);
	std::fill(bytes.begin(), bytes.end(), (unsigned char) 0);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, atlas.atlasWidth_, atlas.atlasHeight_, GL_RGBA, GL_UNSIGNED_BYTE, &bytes[0]);


	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex1_, 0);

#pragma endregion

	GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (e != GL_FRAMEBUFFER_COMPLETE) {
		Darknec::logger("%i", e);
	}


	glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

	glViewport(0, 0, atlas.atlasWidth_, atlas.atlasHeight_);
	glClear(GL_COLOR_BUFFER_BIT);


#pragma region Generate atlas
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas.atlasWidth_, atlas.atlasHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	
	int xPos = 0;
	for (int c = 32; c < atlas.characters_.size(); c++) {
		if (FT_Load_Char(this->fontface_, c, FT_LOAD_RENDER)) {
			continue;
		}
		atlas.characters_[c].xAdvance = glyph->advance.x >> 6;
		atlas.characters_[c].yAdvance = glyph->advance.y >> 6;
		atlas.characters_[c].width = glyph->bitmap.width;
		atlas.characters_[c].height = glyph->bitmap.rows;
		atlas.characters_[c].left = glyph->bitmap_left;
		atlas.characters_[c].top = glyph->bitmap_top;
		atlas.characters_[c].xTexOffset = x;


		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, glyph->bitmap.width, glyph->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
		

		//Make sure the space character has width and height data
		//GRRR float magic numbers
		if (atlas.characters_[c].width < 0.000001f || atlas.characters_[c].height < 0.000001f) {
			atlas.characters_[c].width = atlas.characters_[c].xAdvance;
			atlas.characters_[c].height = atlas.atlasHeight_;
		}
		x += atlas.characters_[c].width;
		xPos += atlas.characters_[c].width + atlas.characters_[c].xAdvance;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
#pragma endregion



	


	float data[16] {
		-1.0, 1.0, 0.0, 1.0,
		1.0, -1.0, 1.0, 0.0,
		1.0, 1.0, 1.0, 1.0,
		-1.0, -1.0, 0.0, 0.0,
	};



	#pragma region Genbuffers
	glGenVertexArrays(1, &this->vao_);
	glBindVertexArray(this->vao_);

	//Create the buffer object.
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &this->vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glGenBuffers(1, &this->ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

#pragma endregion




	//Draw
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture);
	sys3->getShader("Pass").setUniformInt("tex", 2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	//Cleanup
	glDeleteBuffers(1, &this->vao_);
	glDeleteBuffers(1, &this->ibo_);
	glDeleteBuffers(1, &this->vbo_);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//sys3->restorePrevious();
	glBindTexture(GL_TEXTURE_2D, 0);
	return atlas;
}

void ComponentSimpleText::setText(std::string text) {
	this->text_ = text;

	const char* cText = text.c_str();

	int textWidth = 0;
	int textHeight = 0;

	FontAtlas a = this->atlas_;







	for (const char* p = cText; *p; p++) {
		Character c = a.characters_[*p];
		textWidth += c.width;
		if (c.height > textHeight) {
			textHeight = c.height;
		}
	}
	this->textWidth = textWidth;
	this->textHeight = a.atlasHeight_ + 50; //Hack to account for super tall fonts

	glDeleteTextures(1, &this->textTexture_);
	glGenTextures(1, &this->textTexture_);
	glBindTexture(GL_TEXTURE_2D, this->textTexture_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->textWidth, this->textHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo_);
	int xPos = 0;
	for (const char* p = cText; *p; p++) {
		Character c = a.characters_[*p];

		std::vector<unsigned char> bytes = std::vector<unsigned char>(c.width * c.height);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(c.xTexOffset, 0, c.width, c.height, GL_ALPHA, GL_UNSIGNED_BYTE, &bytes[0]);

		int top = this->textHeight - c.top - 50; //50 is magic. Seems to fit all 'regular' fonts and some super ridiculously tall fonts like 'Ghastly Panic' with reasonable leeway top and bottom.

		glTexSubImage2D(GL_TEXTURE_2D, 0, xPos, top, c.width, c.height, GL_ALPHA, GL_UNSIGNED_BYTE, &bytes[0]);
		xPos += c.width;
	}

	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


bool isBuffersCreated = false;

void ComponentSimpleText::Render() {
	sys3->useShader("Text");




	if (!isBuffersCreated) {
		isBuffersCreated = true;
		glGenVertexArrays(1, &this->vao2_);
		glGenBuffers(1, &this->vbo2_);
		glGenBuffers(1, &this->ibo2_);
	}





	float w = this->textWidth;
	float h = this->textHeight;
	float data[16] {
		//Divide by 1000 so that fontSize = 100 is 0.1 for render detail
		0, h/ 1000, 0.0, 0.0,
		w / 1000, 0.0, 1.0, 1.0,
		w / 1000, h / 1000, 1.0, 0.0,
		0, 0.0, 0.0, 1.0,
	};
	

	glBindVertexArray(this->vao2_);

	//Create the buffer object.
	glEnableVertexAttribArray(0);
		
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo2_);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*) 0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo2_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);



	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->textTexture_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo2_);
	sys3->getShader("Text").setUniformInt("tex", 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	//sys3->restorePrevious();
}


XMLNode ComponentSimpleText::write(XMLNode node) {
	return NULL;
}
void ComponentSimpleText::read(XMLNode node) {
	this->transform_ = ComponentRequirement<ComponentTransform>(this->container_, std::string(Darknec::ComponentRWUtils::readString(node, "ComponentRequirement")));
}

void ComponentSimpleText::init() {
	this->transform_.setup();
	bindFont(Darknec::baseAssetPath + "/fonts/Comic_book.ttf");
}

void ComponentSimpleText::update() {


	sys3->useShader("Text");
	glm::mat4 matrix;


	matrix = glm::translate(matrix, this->transform_->getPosition());
	matrix *= glm::mat4_cast(this->transform_->getOrientation());
	matrix = glm::scale(matrix, this->transform_->getScale());
	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(matrix));
	


	this->Render();
}

void ComponentSimpleText::setShader(Shader shader) {
	//this->shader_ = shader;
}