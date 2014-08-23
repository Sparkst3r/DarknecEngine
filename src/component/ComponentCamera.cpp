#include <component/ComponentCamera.h>
#include <component/ComponentTransform.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


void ComponentCamera::read(rapidxml::xml_node<>* node) {
	for (rapidxml::xml_node<>* dataIter = node->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		if (std::string(dataIter->name()) == std::string("ComponentRequirement")) {
			this->positionID_ = std::string(dataIter->value()).c_str();
		}
		else if (std::string(dataIter->name()) == std::string("projectiontype")) {
			std::string type = std::string(dataIter->value());
			if (type == std::string("PERSPECTIVE")) {
				this->projectionType_ = ProjType::PERSPECTIVE;
			}
			else if (type == std::string("ORTHOGRAPHIC")) {
				this->projectionType_ = ProjType::ORTHOGRAPHIC;
			}
		}
		else if (std::string(dataIter->name()) == std::string("FOV")) {
			this->FOV_ = atof(std::string(dataIter->value()).c_str());
		}
		else if (std::string(dataIter->name()) == std::string("width")) {
			this->width_ = atoi(std::string(dataIter->value()).c_str());
		}
		else if (std::string(dataIter->name()) == std::string("height")) {
			this->height_ = atoi(std::string(dataIter->value()).c_str());
		}
		else if (std::string(dataIter->name()) == std::string("nearclip")) {
			this->frustrumNearClip_ = atof(std::string(dataIter->value()).c_str());
		}
		else if (std::string(dataIter->name()) == std::string("farclip")) {
			this->frustrumFarClip_ = atof(std::string(dataIter->value()).c_str());
		}
		else if (std::string(dataIter->name()) == std::string("distance")) {
			this->distance_ = atof(std::string(dataIter->value()).c_str());
		}
	}
}

ComponentCamera::ComponentCamera(GameObject* container) {
	this->container_ = container;
}


glm::mat4 ComponentCamera::generateProjMatrix() {
	if (this->projectionType_ == ProjType::ORTHOGRAPHIC) {
		return glm::ortho(0.0f, (float) width_, (float) height_, 0.0f, frustrumNearClip_, frustrumFarClip_);
	}
	else if (this->projectionType_ == ProjType::PERSPECTIVE) {
		float aspect = (float) width_ / height_;

		if (aspect < 1.0f) {
			aspect = 1.0f;
		}

		return glm::perspective(FOV_, aspect, frustrumNearClip_, frustrumFarClip_);
	}

}
glm::mat4 ComponentCamera::generateViewMatrix() {
	glm::mat4 matrix(1.0f);

	//WARN This can return NULL.
	ComponentTransform* trans = dynamic_cast<ComponentTransform*>(container_->getComponent("transform"));
	
	//0. Distance
	glm::mat4 distance = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -this->distance_));
	//1. Rotate
	glm::mat4 rotate = glm::mat4_cast(glm::angleAxis(0.f, glm::vec3(1.0f, 1.0f, 1.0f)) * trans->getOrientation());
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
void ComponentCamera::setProjType(ComponentCamera::ProjType projType) {
	projectionType_ = projType;
}

float ComponentCamera::getFOV() const {
	return FOV_;
}
void ComponentCamera::setFOV(float FOV) {
	this->FOV_ = FOV;
}

void ComponentCamera::setCaptureDimension(int width, int height) {
	width_ = width;
	height_ = height;
}
void ComponentCamera::setClipPlanes(float nearClip, float farClip) {
	frustrumNearClip_ = nearClip;
	frustrumFarClip_ = farClip;
}