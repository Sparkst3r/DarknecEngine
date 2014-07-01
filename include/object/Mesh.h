#ifndef DARKNEC_MESH_H
#define DARKNEC_MESH_H
#include <render/GLTypeDef.h>
#include <object/fast_tiny_obj_loader.h>

class Mesh {
public:

	Mesh();
	Mesh(const char* filename);

	void Render();
	ObjLoader::Material material;

private:

	VAO vao;
	VBO vertexArrays;
	VBO normalArrays;
	IBO elementArrays;
	int indiciescount;



};

#endif