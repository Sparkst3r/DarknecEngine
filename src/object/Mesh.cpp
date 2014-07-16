#include <object/Mesh.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <SDL.h>
#include <object/ObjLoader.h>
#include <State.h>

Mesh::Mesh() {}

using namespace glm;

#include <soil.h>

//TODO Temp UV until loading is fixed properly

static const GLfloat g_uv_buffer_data[] = {
	0.000059f, 1.0f - 0.000004f,
	0.000103f, 1.0f - 0.336048f,
	0.335973f, 1.0f - 0.335903f,
	1.000023f, 1.0f - 0.000013f,
	1.000023f, 1.0f - 0.000013f,
	0.668104f, 1.0f - 0.000013f,
	0.668104f, 1.0f - 0.000013f,
	0.335973f, 1.0f - 0.335903f,
};

Mesh::Mesh(const char* filename) {

	bool write = 0;
	
	std::vector<ObjLoader::Shape> shapes;
	if (write) {
		ObjLoader::LoadObj(shapes, filename, "assets/");

		CObjLoader::write(std::ofstream(std::string(filename) + "test.cobj", std::ios::binary), shapes);
	}
	else {
		shapes = CObjLoader::read(std::ifstream(std::string(filename) + "test.cobj", std::ios::binary));

	}


	std::vector<float> verticest = shapes[0].mesh.vertices;
	std::vector<float> uvst = shapes[0].mesh.texcoords;
	std::vector<float> normalst = shapes[0].mesh.normals;
	std::vector<unsigned int> indiciest = shapes[0].mesh.indices;
	
	material = shapes[0].material;

	/*ObjLoader::loadObj(filename, vertices, uvs, normals, indicies);*/

	this->indiciescount = indiciest.size();


	GLuint texture = SOIL_load_OGL_texture("assets/texture.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

		//Create the buffer object.
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &this->vertexArrays);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexArrays);
	glBufferData(GL_ARRAY_BUFFER, verticest.size() * sizeof(float), &verticest[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	//Create the buffer object.
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &this->normalArrays);
	glBindBuffer(GL_ARRAY_BUFFER, this->normalArrays);
	glBufferData(GL_ARRAY_BUFFER, normalst.size() * sizeof(float), &normalst[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	
	VBO uv;

	std::vector<GLfloat> uvsy;

	for (GLfloat uv : g_uv_buffer_data) {
		uvsy.push_back(uv);
	}

	
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &uv);
	glBindBuffer(GL_ARRAY_BUFFER, uv);
	glBufferData(GL_ARRAY_BUFFER, uvsy.size() * sizeof(float), &uvsy[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	glGenBuffers(1, &this->elementArrays);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementArrays);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiciest.size() * sizeof(unsigned int), &indiciest[0], GL_STATIC_DRAW);

	

	glBindVertexArray(0);
}

void Mesh::Render() {
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementArrays);
	glDrawElements(GL_TRIANGLES, indiciescount, GL_UNSIGNED_INT, 0);
}
