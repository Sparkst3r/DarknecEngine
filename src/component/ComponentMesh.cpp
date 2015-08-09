#include <component/ComponentMesh.h>
#include <system/ShaderSystem.h>
#include <assimp/Importer.hpp>   
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>    
#include <assimp/postprocess.h>
#include <soil.h>

ComponentMesh::ComponentMesh(GameObject* container) {
	container_ = container;
}

XMLNode ComponentMesh::write(XMLNode node) {
	return NULL;
}
void ComponentMesh::read(XMLNode node) {
	this->transform_ = Darknec::ComponentRWUtils::readRequirement<ComponentTransform>(node, std::string("ComponentTransform"), std::string("transform"), this->container_);
	std::string str = "";
	for (rapidxml::xml_node<>* dataIter = node->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		if (std::string(dataIter->name()) == std::string("mesh")) {
			str = Darknec::ComponentRWUtils::readString(dataIter, "geometry");
		}
	}

	if (!str.empty()) {
		this->modelFile_ = std::string(Darknec::baseAssetPath + str);
	}
	else {
		Darknec::logger("ComponentMesh", Darknec::LOG_WARN, "ComponentMesh '%s' in object '%s' has mesh with no geometry. Skipping", this->name_.c_str(), this->container_->getName().c_str());
	}
	
}
bool ComponentMesh::validate() {
	return this->transform_.validate() && this->transform_->validate();
}



void ComponentMesh::init() {
	this->transform_.setup();

	if (!this->modelFile_.empty()) {
		Assimp::Importer importer;
		aiScene* scene = NULL;

		if (std::ifstream(this->modelFile_ + ".cobj").good()) {
			this->model_ = Darknec::CObjLoader::read(std::string(this->modelFile_) + ".cobj");
			Darknec::logger("ComponentMesh", Darknec::LogLevel::LOG_LOG, "Loaded compiled object: %s", std::string(std::string(this->modelFile_) + ".cobj").c_str());
		}
		else if (std::ifstream(std::string(this->modelFile_)).good()) {
			
			unsigned int postProcessFlags =
				aiProcess_CalcTangentSpace |
				aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType;

			scene = const_cast <aiScene*>(importer.ReadFile(this->modelFile_, postProcessFlags));
			Darknec::logger("ComponentMesh", Darknec::LogLevel::LOG_LOG, "Found object %s; loading", this->modelFile_.c_str());

			
			model_ = Model::ConvertAssimpToDarknec(scene);
			Darknec::CObjLoader::write(std::ofstream(this->modelFile_ + ".cobj", std::ios::binary), this->model_);
			Darknec::logger("ComponentMesh", Darknec::LogLevel::LOG_LOG, "Compiled %s", this->modelFile_.c_str());
		}
		else {
			Darknec::logger("ComponentMesh", Darknec::LogLevel::LOG_ERROR, "Could not load object %s", this->modelFile_.c_str());
		}
		sys3->useShader("Phong");

		for (int mesh = 0; mesh < model_.meshes_.size(); mesh++) {
			Mesh* currMesh = &model_.meshes_[mesh];
			currMesh->setupGLBuffers();

		}
	}
	else {
		Darknec::logger("ComponentMesh", Darknec::LogLevel::LOG_WARN, "Model file in GameObject: %s is undefined.", this->container_->getName().c_str());
	}
}

void ComponentMesh::update() {
	sys3->useShader("Text");


	glm::mat4 matrix;
	matrix = glm::scale(matrix, this->transform_->getScale());

	matrix = glm::translate(matrix, this->transform_->getPosition());
	matrix *= glm::mat4_cast(this->transform_->getOrientation());

	glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(matrix));
	renderer->render(this->model_);
}


