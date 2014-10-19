#include <component/ComponentCamera.h>


void ComponentCamera::read(rapidxml::xml_node<>* node) {
	this->transform_ = ComponentRequirement<ComponentTransform>(this->container_, std::string("transform"));


	std::string projectionType = Darknec::ComponentRWUtils::readString(node, "projectiontype");
	if (projectionType == std::string("PERSPECTIVE")) {
		this->projectionType_ = ProjType::PERSPECTIVE;
	}
	else if (projectionType == std::string("ORTHOGRAPHIC")) {
		this->projectionType_ = ProjType::ORTHOGRAPHIC;
	}

	this->FOV_ =				Darknec::ComponentRWUtils::readFloat(node, "FOV");
	this->width_ =				Darknec::ComponentRWUtils::readInt(node, "width");
	this->height_ =				Darknec::ComponentRWUtils::readInt(node, "height");
	this->frustrumNearClip_ =	Darknec::ComponentRWUtils::readFloat(node, "nearclip");
	this->frustrumFarClip_ =	Darknec::ComponentRWUtils::readFloat(node, "farclip");
	this->distance_ =			Darknec::ComponentRWUtils::readFloat(node, "distance");
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

	////0. Distance
	glm::mat4 distance = glm::translate(matrix, glm::vec3(0.0f, 0.0f, -this->distance_));
	////1. Rotate
	glm::mat4 rotate = glm::mat4_cast(glm::angleAxis(0.f, glm::vec3(1.0f, 1.0f, 1.0f)) * transform_->getOrientation());
	////2. Translate
	glm::mat4 translate = glm::translate(matrix, -transform_->getPosition());

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