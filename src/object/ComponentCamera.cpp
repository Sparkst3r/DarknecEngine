#include <object/ComponentCamera.h>
#include <object/ComponentTransform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


ComponentCamera::ComponentCamera(GameObject* container, const char* positionID) {
	this->container_ = container;
	this->positionID_ = positionID;
	this->componentLogger = container->componentLogger;
}


glm::mat4 ComponentCamera::generateProjMatrix() {
	//TODO fix for orthographic projection.
	
	return glm::perspective(FOV_, (float)(width_ / height_), frustrumNearClip_, frustrumFarClip_);
}
glm::mat4 ComponentCamera::generateViewMatrix() {
	glm::mat4 matrix(1.0f);

	//WARN This can return NULL.
	ComponentTransform* trans = dynamic_cast<ComponentTransform*>(container_->getComponent("transform"));
	
	//0. Distance
	glm::mat4 distance = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -this->distance_));
	//1. Rotate
	glm::mat4 rotate = glm::mat4_cast(glm::angleAxis(0.f, glm::vec3(0.0f, 0.0f, 1.0f)) * trans->getOrientation());
	//2. Translate
	glm::mat4 translate = glm::translate(matrix, -trans->getPosition());

	return distance * rotate * translate;
}

float ComponentCamera::getDistance() const {
	return this->distance_;
}
void ComponentCamera::setDistance(float distance) {
	this->distance_ = distance;
}
void ComponentCamera::offsetDistance(float offsetDistance) {
	this->distance_ += offsetDistance;
}

bool ComponentCamera::getProjType() const {
	return projectionType_;
}
void ComponentCamera::setProjType(bool type) {
	projectionType_ = type;
}

float ComponentCamera::getFOV() const {
	return FOV_;
}
void ComponentCamera::setFOV(float FOV) {
	FOV_ = FOV;
}

void ComponentCamera::setCaptureDimension(int width, int height) {
	width_ = width;
	height_ = height;
}
void ComponentCamera::setClipPlanes(float nearClip, float farClip) {
	frustrumNearClip_ = nearClip;
	frustrumFarClip_ = farClip;
}