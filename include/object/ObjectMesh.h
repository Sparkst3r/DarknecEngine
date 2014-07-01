#ifndef DARKNEC_OBJECTMESH_H
#define DARKNEC_OBJECTMESH_H

#include <SDL.h>
#include <object/Object.h>
#include <object/Mesh.h>
#include <render/shader/Shader.h>

class ObjectMesh : public Object {

public:

	ObjectMesh(Mesh* mesh, Shader shader, UBO matrices);
	~ObjectMesh();

	void renderObject();

private:
	Mesh* model = NULL;
	Shader shader;
	UBO matrices;

};


#endif