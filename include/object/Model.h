#ifndef DARKNEC_MODEL_H

#include <vector>
#include <render/GLTypeDef.h>
#include <glm/glm.hpp>

class Texture {

private:
	const char* filename;

	const char* data;

};

class Material {

	float ambient;
	float diffuse;
	float specular;

	Texture diffuseTex;
	Texture specularTex;
	Texture normalTex;
	Texture displacementTex;


};

class Mesh {
public:
	struct Vertex {
		GLuint vertex;
		GLuint normal;
		GLuint texcoord;
	};


	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	
};


class Armature {};


class Model {
	enum FileType {
		OBJ,
		COLLADA,
	};

	Model(const char* filename, FileType type);


private:

	void loadObj(FILE* file);

	Mesh meshes;

	Armature armature;

};
#endif // !DARKNEC_MODEL_H
