#include <object/ObjectCamera.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ObjectCamera::ObjectCamera(glm::vec3 position, glm::fquat rotation) : ObjectCamera(position, rotation, 5.0f) {}

ObjectCamera::ObjectCamera(glm::vec3 position, glm::fquat rotation, float distance) : Object(position, rotation) {
	this->distance = distance;
}


glm::mat4 ObjectCamera::generateViewMatrix() {
	glm::mat4 matrix(1.0f);

	//0. Distance
	glm::mat4 distance = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -this->distance));
	//1. Scale
	glm::mat4 scale = glm::scale(matrix, this->getScale());
	//2. Rotate
	glm::mat4 rotate = glm::mat4_cast(glm::angleAxis(0.f, glm::vec3(0.0f, 0.0f, 1.0f)) *this->getOrientation());
	//3. Translate
	glm::mat4 translate = glm::translate(matrix, -this->getPosition());


	return distance * scale * rotate * translate;
}
glm::mat4 ObjectCamera::generateProjectionMatrix() {
	return glm::perspective(fov, (float) (width / height), nearClippingPlane, farClippingPlane);
}




void  ObjectCamera::offsetDistance(float offset) {
	this->setDistance(this->distance + offset);
}

float ObjectCamera::getDistance() const {
	return this->distance;
}
int   ObjectCamera::getWidth() const {
	return this->width;
}
int   ObjectCamera::getHeight() const {
	return this->height;
}

void  ObjectCamera::setDistance(float distance) {
	this->distance = distance;
}
void  ObjectCamera::setProjectionBounds(int width, int height) {
	this->width = width;
	this->height = height;
}
void  ObjectCamera::setProjectionClipBounds(float nearClip, float farClip) {
	this->nearClippingPlane = nearClip;
	this->farClippingPlane = farClip;
}
void  ObjectCamera::setFieldOfView(float FOV) {
	this->fov = FOV;
}