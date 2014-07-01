#include <object/ComponentTransform.h>

ComponentTransform::ComponentTransform(GameObject* container) {
	this->container_ = container;
	this->componentLogger = container->componentLogger;
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