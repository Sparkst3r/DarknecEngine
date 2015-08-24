#include <component/ComponentMesh.h>
#include <system/ShaderSystem.h>
#include <assimp/Importer.hpp>   
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>    
#include <assimp/postprocess.h>
#include <soil.h>

ComponentMesh::ComponentMesh(GameObject* container) {
	container_ = container;
	this->typeName_ = "ComponentMesh";
}

XMLNode ComponentMesh::write(XMLNode node) {
	return NULL;
}
void ComponentMesh::read(XMLNode node) {
	this->transform_ = Darknec::ComponentRWUtils::readRequirement<ComponentTransform>(node, std::string("ComponentTransform"), std::string("transform"), this->container_);
	for (rapidxml::xml_node<>* dataIter = node->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		if (std::string(dataIter->name()) == std::string("material")) {
			Material mat;

			for (rapidxml::xml_node<>* dataIter2 = dataIter->first_node(); dataIter2; dataIter2 = dataIter2->next_sibling()) {
				if (std::string(dataIter2->name()) == std::string("texture")) {
					std::string type = dataIter2->first_attribute()->value();
					if (type == std::string("DIFFUSE")) {
						mat.set(Material::TEXTURE_DIFFUSE, Texture(Darknec::baseAssetPath + std::string(dataIter2->value())));
					}
				}
				else if (std::string(dataIter2->name()) == std::string("colour")) {
					std::string type = dataIter2->first_attribute()->value();
					if (type == std::string("AMBIENT")) {
						mat.set(Material::COLOUR_AMBIENT, Darknec::ComponentRWUtils::readVector4(dataIter2, "channel", "R", "G", "B", "A"));
					}
					else if (type == std::string("DIFFUSE")) {
						mat.set(Material::COLOUR_DIFFUSE, Darknec::ComponentRWUtils::readVector4(dataIter2, "channel", "R", "G", "B", "A"));
					}
					else if (type == std::string("SPECULAR")) {
						mat.set(Material::COLOUR_SPECULAR, Darknec::ComponentRWUtils::readVector4(dataIter2, "channel", "R", "G", "B", "A"));
					}
				}

			}

			std::string name = dataIter->first_attribute()->value();
			this->model_.materials_.push_back(mat);
			this->model_.matLookup_[name] = this->model_.materials_.size()-1;
		}
	}
	
	for (rapidxml::xml_node<>* dataIter = node->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		if (std::string(dataIter->name()) == std::string("mesh")) {
			int id = atoi(dataIter->first_attribute()->value());
			if (this->model_.meshMats_.size() < id + 1) {
				this->model_.meshMats_.resize(id + 1);
			}
			this->model_.meshMats_[id] = this->model_.matLookup_[Darknec::ComponentRWUtils::readString(dataIter, "material")];
		}
	}
	
	std::string str = "";
	str = Darknec::ComponentRWUtils::readString(node, "model");


	if (!str.empty()) {
		this->modelFile_ = std::string(Darknec::baseAssetPath + str);
	}
	else {
		Darknec::logger("ComponentMesh", Darknec::LOG_WARN, "ComponentMesh '%s' in object '%s' has no model. Skipping", this->name_.c_str(), this->container_->getName().c_str());
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
			this->model_ = Darknec::CObjLoader::read(model_, std::string(this->modelFile_) + ".cobj");
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

			
			this->model_ = Model::ConvertAssimpToDarknec(this->model_, scene);
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
			//if (model_.meshMats_.size() > mesh) {
				currMesh->materialIndex_ = model_.meshMats_[0];
			//}
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

Renderer* renderer = new RendererDeferredPhong();

