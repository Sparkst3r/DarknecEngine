#ifndef DARKNEC_TEXTURE_H 
#define DARKNEC_TEXTURE_H

#include <Core.h>


class Texture {

public:
	Texture();
	Texture(std::string file);

	void bind(GLuint texUnit);
	void bindHidden();

	virtual ~Texture();
	GLuint glTexID_;
private:


};

#endif // !DARKNEC_TEXTURE_H
