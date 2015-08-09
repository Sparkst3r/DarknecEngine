#include <component/ComponentInput.h>
#include <system/InputSystem.h>
#include <component/ComponentCamera.h>

/**
* ComponentCamera
*
* Constructor
*/
ComponentInput::ComponentInput(GameObject* container) {
	this->container_ = container;
}

bool ComponentInput::validate() {
	return true;
}

void ComponentInput::init() {
	sys4->registerInputReceiver(this, type);
}

void ComponentInput::OffsetCameraDebug(glm::vec3 offset) {
	if (container_->hasComponent("camera")) {
		ComponentCamera* camera2 = container_->getCastComponent<ComponentCamera>("camera");
		if (camera2 != NULL) {
			glm::fquat orientation = glm::quat_cast(camera2->generateViewMatrix());

			glm::fquat invOrient = glm::conjugate(orientation);
			glm::vec3 worldOffset = invOrient * offset;
			ComponentTransform* transform2 = container_->getCastComponent<ComponentTransform>("transform");
			if (transform2 != NULL) {
				transform2->offsetPosition(worldOffset);
			}
		}
	}
}

bool grabbing = false;
bool cangrab = true;

void ComponentInput::update() {
	if (this->type == MOUSE) {
		if (container_->hasComponent("camera") && grabbing) {

			float degXAngleDiff = (float(Darknec::WindowWidth / 2 - sys4->mouse_X_) * 0.4f);
			float degYAngleDiff = (float(Darknec::WindowHeight / 2 - sys4->mouse_Y_) * 0.4f);

			ComponentTransform* transform2 = container_->getCastComponent<ComponentTransform>("transform");
			if (transform2 != NULL) {
				transform2->setOrientation(transform2->getOrientation() * glm::angleAxis(-degXAngleDiff, glm::vec3(0.0f, 1.0f, 0.0f)));

				//Rotate about the local-space X axis.
				transform2->setOrientation(glm::angleAxis(-degYAngleDiff, glm::vec3(1.0f, 0.0f, 0.0f)) * transform2->getOrientation());
			}
		}
	}
	else {
		glm::vec3 direction = glm::vec3();
		if (sys4->keysPressed[GLFW_KEY_A]) {
			glm::vec3 direction = glm::vec3();
			direction.x -= 0.1f;
		}
		if (sys4->keysPressed[GLFW_KEY_A]) {
			direction.x -= 1.0f;
		}
		if (sys4->keysPressed[GLFW_KEY_D]) {
			direction.x += 1.0f;
		}
		if (sys4->keysPressed[GLFW_KEY_S]) {
			direction.z += 1.0f;
		}
		if (sys4->keysPressed[GLFW_KEY_W]) {
			direction.z -= 1.0f;
		}

		if (sys4->keysPressed[GLFW_KEY_SPACE]) {
			direction.y += 1.0f;
		}
		if (sys4->keysPressed[GLFW_KEY_LEFT_SHIFT]) {
			direction.y -= 1.0f;
		}
		if (sys4->keysPressed[GLFW_KEY_ESCAPE]) {
			Darknec::RUNSTATE = Darknec::STOPPED;
		}
		if (sys4->keysPressed[GLFW_KEY_G] && cangrab) {
			cangrab = false;
			grabbing = !grabbing;
			if (grabbing) {

				glfwSetInputMode(Darknec::Detail::window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			else {
				glfwSetInputMode(Darknec::Detail::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
		}
		if (sys4->lastPressMode == GLFW_RELEASE) {
			cangrab = true;
		}

		direction *= 0.4f;
		OffsetCameraDebug(direction);
	}

	if (grabbing) {
		glfwSetCursorPos(Darknec::Detail::window, Darknec::WindowWidth / 2, Darknec::WindowHeight / 2);
	}

}
/**
* read
*
* Load component data from xml.
* @param node root node of the component to read from.
*/
void ComponentInput::read(XMLNode node) {
	std::string type = Darknec::ComponentRWUtils::readString(node, "type");
	if (type == "KEYBOARD") {
		this->type = InputType::KEYBOARD;
	}
	else if (type == "MOUSE") {
		this->type = InputType::MOUSE;

	}

}