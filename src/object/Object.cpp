#include <object/Object.h>


	Object::Object() {
		this->setPosition(glm::vec3(0, 0, 0));
		this->setOrientation(glm::fquat(0.0f, 0.0f, 0.0f, 0.0f));
		this->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	}

	Object::Object(glm::vec3 position) {
		this->setPosition(position);
		this->setOrientation(glm::fquat(0.0f, 0.0f, 0.0f, 0.0f));
		this->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	}

	Object::Object(glm::fquat rotation) {
		this->setPosition(glm::vec3(0, 0, 0));
		this->setOrientation(rotation);
		this->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	}
	
	Object::Object(glm::vec3 position, glm::fquat rotation) {
		this->setPosition(position);
		this->setOrientation(rotation);
		this->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	}

	void Object::setPosition(glm::vec3 position) {
		this->position = position;
	}

	void Object::updatePosition() {}


