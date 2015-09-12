#include <render/Texture.h>

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <State.h>

Texture::Texture() {}

Texture::Texture(GLuint type) {
	this->type_ = type;
	this->glTexID_ = 0;
	glGenTextures(1, &this->glTexID_);

}

void Texture::setData(std::string file) {
	int width, height, numComponents;
	unsigned char* data = stbi_load((file).c_str(), &width, &height, &numComponents, 4);
	this->width_ = width;
	this->height_ = height;
	if (data == NULL) {
		Darknec::logger(Darknec::LOG_WARN, "Texture file not found or could not be read. File path: %s", file.c_str());
	}
	this->data_ = data;
}
void Texture::setData(void* data) {
	this->data_ = data;

}

void Texture::setDims(GLuint width, GLuint height) {
	this->width_ = width;
	this->height_ = height;
}
void Texture::setValueType(GLuint type) {
	this->valueType_ = type;
}

void Texture::setFormat(GLuint texelformat, GLuint internalFormat) {
	this->intFormat_ = internalFormat;
	this->texFormat_ = texelformat;
}

void Texture::setParameter(GLuint texParam, GLuint value) {
	this->bindHidden();
	glTexParameteri(this->type_, texParam, value);
}

void Texture::create() {
	glEnable(GL_TEXTURE_2D);
	this->bindHidden();
	glTexImage2D(this->type_, this->mipmapLevel_, this->intFormat_, this->width_, this->height_, 0, this->texFormat_, this->valueType_, data_);
}

void Texture::destroy() {

	if (this->data_ != NULL) {
		delete[] this->data_;
	}
	
	glDeleteTextures(1, &this->glTexID_);
	this->glTexID_ = 0;
}

void Texture::bind(GLuint texUnit) {
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, this->glTexID_);
}

void Texture::bindHidden() {
	glBindTexture(GL_TEXTURE_2D, this->glTexID_);
}

void Texture::bindToFrameBuffer(GLuint attachment, GLuint target, GLuint mipmapLevel) {
	this->bindHidden();
	glFramebufferTexture2D(target, attachment, this->type_, this->glTexID_, mipmapLevel);
}