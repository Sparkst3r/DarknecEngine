#ifndef DARKNEC_OBJECT_H
#define DARKNEC_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Object {
protected:
	glm::vec3 position;
	glm::fquat orientation;
	glm::vec3 scale;

public:
	Object();

	Object(glm::vec3 position);

	Object(glm::fquat rotation);

	Object(glm::vec3 position, glm::fquat rotation);


	glm::vec3 getPosition() {
		return position;
	}

	glm::fquat getOrientation()  {
		return orientation;
	}

	glm::vec3 getScale()  {
		return scale;
	}

	virtual void setPosition(glm::vec3 position);

	void setOrientation(glm::fquat orientation) {
		this->orientation = orientation;
	}

	void setScale(glm::vec3 scale) {
		this->scale = scale;
	}

	void offsetPosition(glm::vec3 offset) {
		this->position += offset;
	}

	virtual void updatePosition();

};

#endif