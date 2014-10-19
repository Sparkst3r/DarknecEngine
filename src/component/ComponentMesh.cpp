#include <component/ComponentMesh.h>

ComponentMesh::ComponentMesh(GameObject* container) {
	container_ = container;
}

rapidxml::xml_node<>* ComponentMesh::write(rapidxml::xml_node<>* node) {
	return NULL;
}
void ComponentMesh::read(rapidxml::xml_node<>* node) {
	this->trans = Darknec::ComponentRWUtils::readString(node, "ComponentRequirement");
	this->model_ = new Mesh(Darknec::ComponentRWUtils::readString(node, "mesh").c_str());
}
void ComponentMesh::update() {

}

void ComponentMesh::renderObject() {
	ComponentTransform* transform = container_->getCastComponent<ComponentTransform>(std::string("transform"));


	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);


	glm::mat4 matrix;
	matrix = glm::scale(matrix, transform->getScale());

	matrix = glm::translate(matrix, transform->getPosition());
	matrix *= glm::mat4_cast(transform->getOrientation());

	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(matrix));

	UnifID uniformID1 = glGetUniformLocation(id, "frontMaterial.ambient");
	UnifID uniformID2 = glGetUniformLocation(id, "frontMaterial.diffuse");
	UnifID uniformID3 = glGetUniformLocation(id, "frontMaterial.specular");
	UnifID uniformID4 = glGetUniformLocation(id, "frontMaterial.shininess");

	glUniform4f(uniformID1, 0.2f, 0.3f, 0.9f, 1.0f);
	glUniform4f(uniformID2, 0.2f, 0.3f, 0.9f, 1.0f);
	glUniform4f(uniformID3, 0.2f, 0.3f, 0.9f, 1.0f);
	glUniform1f(uniformID4, 10.0f);

	model_->Render();


}

void ComponentMesh::setShader(Shader* shader) {
	this->shader_ = shader;

}
void ComponentMesh::setMesh(Mesh* model) {
	this->model_ = model;
}
void ComponentMesh::setUBO(UBO matrices) {
	this->matrices_ = matrices;
}

void ComponentMesh::setTransform(std::string str) {
	this->trans = str;
}