#include <component/ComponentPhysics.h>

ComponentPhysics::ComponentPhysics(GameObject* container, const char* positionID) : positionID_(positionID) {
	container_ = container;
}

void ComponentPhysics::read(rapidxml::xml_node<>* node) {
	for (rapidxml::xml_node<>* dataIter = node->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		if (std::string(dataIter->name()) == std::string("position")) {
		
		}
	}
}

void ComponentPhysics::update() {
	Component* component = container_->getComponent(positionID_);

	ComponentTransform* transform = dynamic_cast<ComponentTransform*>(component);

	if (transform != NULL) { //Take bullet position for opengl
		btTransform trans;
		fallRigidBody_->getMotionState()->getWorldTransform(trans);
		btVector3 p = trans.getOrigin();
		btQuaternion o = trans.getRotation();
		transform->setPosition(glm::vec3(p.getX(), p.getY(), p.getZ()));
		transform->setOrientation(glm::fquat(o.getX(), o.getY(), o.getZ(), o.getW()));
	}

}