#include <component/ComponentTransform.h>

ComponentTransform::ComponentTransform(GameObject* container) {
	this->container_ = container;
}

void ComponentTransform::read(rapidxml::xml_node<>* node) {
	for (rapidxml::xml_node<>* dataIter = node->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		if (std::string(dataIter->name()) == std::string("position")) {


			rapidxml::xml_attribute<char>* positionComp = dataIter->first_attribute();
			std::string s = std::string(dataIter->value());
			///// This entire code chunk is ugly as &*%@. This can surely be done in a for loop. Some day.///////
			if (std::string(positionComp->value()) == std::string("X")) {
				this->position_.x = atof(s.c_str());
			}
			else if (std::string(positionComp->value()) == std::string("Y")) {
				this->position_.y = atof(s.c_str());
			}
			else if (std::string(positionComp->value()) == std::string("Z")) {
				this->position_.z = atof(s.c_str());
			}
		}
		else if (std::string(dataIter->name()) == std::string("orientation")) {
			rapidxml::xml_attribute<char>* orientComp = dataIter->first_attribute();
			std::string s = std::string(dataIter->value());

			///// This entire code chunk is ugly. This can surely be done in a for loop. Some day.///////


			if (std::string(orientComp->value()) == std::string("X")) {
				this->orient_.x = atof(s.c_str());
			}
			else if (std::string(orientComp->value()) == std::string("Y")) {
				this->orient_.y = atof(s.c_str());
			}
			else if (std::string(orientComp->value()) == std::string("Z")) {
				this->orient_.z = atof(s.c_str());
			}
			else if (std::string(orientComp->value()) == std::string("W")) {
				this->orient_.w = atof(s.c_str());
			}
		}
	}
}


glm::vec3 ComponentTransform::getPosition() const {
	return this->position_;
}
glm::fquat ComponentTransform::getOrientation() const {
	return this->orient_;
}
glm::vec3 ComponentTransform::getScale() const {
	return this->scale_;
}

void ComponentTransform::setPosition(glm::vec3 position) {
	this->position_ = position;
}
void ComponentTransform::setOrientation(glm::fquat orient) {
	this->orient_ = orient;
}
void ComponentTransform::setScale(glm::vec3 scale) {
	this->scale_ = scale;
}

void ComponentTransform::offsetPosition(glm::vec3 positionOffset) {
	this->position_ += positionOffset;
}
void ComponentTransform::offsetScale(glm::vec3 scaleOffset) {
	this->scale_ += scaleOffset;
}