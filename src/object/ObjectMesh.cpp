#include <object/ObjectMesh.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <sstream>

ObjectMesh::ObjectMesh(Mesh* mesh, Shader* shader, UBO matrices) : model(mesh), shader(shader) {
}

ObjectMesh::~ObjectMesh() {
	delete model;
}

void ObjectMesh::renderObject() {
	glm::mat4 matrix;
	matrix = glm::scale(matrix, this->getScale());


	matrix = glm::translate(matrix, this->getPosition());
	matrix *= glm::mat4_cast(this->getOrientation());

	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(matrix));
	

	glUniform4f(shader->getUniform("frontMaterial.ambient"), model->material.ambient[0], model->material.ambient[1], model->material.ambient[2], 1.0f);
	glUniform4f(shader->getUniform("frontMaterial.diffuse"), model->material.diffuse[0], model->material.diffuse[1], model->material.diffuse[2], 1.0f);
	glUniform4f(shader->getUniform("frontMaterial.specular"), model->material.specular[0], model->material.specular[1], model->material.specular[2], 1.0f);
	glUniform1f(shader->getUniform("frontMaterial.shininess"), 25.0);
	
	model->Render();
}