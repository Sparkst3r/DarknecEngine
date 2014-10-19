#include <object/Mesh.h>
#include <State.h>
#include <assimp/Importer.hpp>   
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>    
#include <assimp/postprocess.h>
#include <object/CObjLoader.h>

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
	Assimp::Importer importer;
	Model model;

	aiScene* scene = NULL;

	if (std::ifstream(std::string(filename) + ".cobj").good()) {
		Uint32 start = SDL_GetTicks();
		model = Darknec::CObjLoader::read(std::ifstream(std::string(filename) + ".cobj", std::ios::binary));
	}
	else if (std::ifstream(std::string(filename)).good()) {
		Uint32 start = SDL_GetTicks();

		scene = const_cast <aiScene*>(importer.ReadFile(filename,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType));

		model = Model::ConvertAssimpToDarknec(scene);
		Darknec::CObjLoader::write(std::ofstream(std::string(filename) + ".cobj", std::ios::binary), model);
	
	
	}
	else {
		Darknec::logger("Sad face :(");
	}



	Darknec::logger("%i", model.meshes_.size());

	std::vector<float> verticest = model.meshes_[0].vertices_;
	std::vector<float> normalst = model.meshes_[0].normals_;
	std::vector<unsigned int> indiciest = model.meshes_[0].indices_;




	
	//material = shapes[0].material;

	/*ObjLoader::loadObj(filename, vertices, uvs, normals, indicies);*/

	this->indiciescount = indiciest.size();


	GLuint texture = SOIL_load_OGL_texture("assets/texture2.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
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

	std::vector<float> uvsy;

	if (model.meshes_[0].hasUVS_ && model.meshes_[0].UVs_.size() != 0) {
		uvsy = model.meshes_[0].UVs_;
	}
	else {
		for (GLfloat uv : g_uv_buffer_data) {
			uvsy.push_back(uv);
		}
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
