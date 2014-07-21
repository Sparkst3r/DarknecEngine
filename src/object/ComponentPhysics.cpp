#include <object/ComponentPhysics.h>

#include <GLMBullet.h>

ComponentPhysics::ComponentPhysics(GameObject* container, const char* positionID) : positionID_(positionID) {
	container_ = container;
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