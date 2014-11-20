#include <component/ComponentMesh.h>
#include <system/ShaderSystem.h>

ComponentMesh::ComponentMesh(GameObject* container) {
	container_ = container;
}

rapidxml::xml_node<>* ComponentMesh::write(rapidxml::xml_node<>* node) {
	return NULL;
}
void ComponentMesh::read(rapidxml::xml_node<>* node) {
	this->trans = Darknec::ComponentRWUtils::readString(node, "ComponentRequirement");
	this->model_ = new Mesh(std::string(Darknec::baseAssetPath + Darknec::ComponentRWUtils::readString(node, "mesh")).c_str());
}
void ComponentMesh::update() {
	ComponentTransform* transform = container_->getCastComponent<ComponentTransform>(std::string("transform"));

	sys3->useShader("Phong");


	glm::mat4 matrix;
	matrix = glm::scale(matrix, transform->getScale());

	matrix = glm::translate(matrix, transform->getPosition());
	matrix *= glm::mat4_cast(transform->getOrientation());

	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(matrix));

	Shader s = sys3->getShader("Phong");

	s.setUniformFloat4("frontMaterial.ambient", 0.2f, 0.3f, 0.9f, 1.0f);
	s.setUniformFloat4("frontMaterial.diffuse", 0.2f, 0.3f, 0.9f, 1.0f);
	s.setUniformFloat4("frontMaterial.specular", 0.2f, 0.3f, 0.9f, 1.0f);
	s.setUniformFloat("frontMaterial.shininess", 20.0f);

	model_->Render();

	//sys3->restorePrevious();
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