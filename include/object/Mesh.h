#ifndef DARKNEC_MESH_H
#define DARKNEC_MESH_H

#include <render/shader/Shader.h>
#include <object/CObjLoader.h>

class Mesh {
public:

	Mesh();
	Mesh(const char* filename);

	void Render();
//	ObjLoader::Material material;

private:

	VAO vao;
	VBO vertexArrays;
	VBO normalArrays;
	IBO elementArrays;
	int indiciescount;



};

#endif