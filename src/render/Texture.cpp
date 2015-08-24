#include <render/Texture.h>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <State.h>

Texture::Texture() {
	this->glTexID_ = -1;
}

Texture::Texture(std::string file) {
	int width, height, numComponents;
	unsigned char* data = stbi_load((file).c_str(), &width, &height, &numComponents, 4);



	if (data == NULL) {
		Darknec::logger(Darknec::LOG_ERROR, "File %s", file.c_str());
	}
	glGenTextures(1, &this->glTexID_);
	glBindTexture(GL_TEXTURE_2D, glTexID_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	Darknec::logger("%i", glTexID_);
}

void Texture::bind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->glTexID_);
}

void Texture::bindHidden() {
	glBindTexture(GL_TEXTURE_2D, this->glTexID_);
}

Texture::~Texture() {
}