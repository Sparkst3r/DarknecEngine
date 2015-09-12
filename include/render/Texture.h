#ifndef DARKNEC_TEXTURE_H 
#define DARKNEC_TEXTURE_H

#include <Core.h>


class Texture {

public:
	
	Texture(GLuint type);
	Texture();

	void setData(std::string file);
	void setData(void* data);
	void setDims(GLuint width, GLuint height);
	void setValueType(GLuint type);
	void setFormat(GLuint texelformat, GLuint internalFormat);

	void setParameter(GLuint texParam, GLuint value);

	void create();

	void bind(GLuint texUnit);
	void bindHidden();

	void bindToFrameBuffer(GLuint attachment, GLuint target = GL_FRAMEBUFFER, GLuint mipmapLevel = 0);

	void destroy();
	void* data_ = NULL;


	GLuint glTexID_ = 0;
	GLuint type_ = GL_TEXTURE_2D;
	GLuint width_ = 0;
	GLuint height_ = 0;
	GLuint valueType_ = 0;
	GLuint intFormat_ = 0;
	GLuint texFormat_ = 0;
	GLuint mipmapLevel_ = 0;

private:



};

#endif // !DARKNEC_TEXTURE_H
