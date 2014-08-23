#include <component/ComponentPhysics.h>

ComponentPhysics::ComponentPhysics(GameObject* container) {
	container_ = container;
}

void ComponentPhysics::read(rapidxml::xml_node<>* node) {
	this->collisionShape_ = new btSphereShape(1);
	btVector3 fallInertia(0, 0, 0);
	this->collisionShape_->calculateLocalInertia(1, fallInertia);
	this->state_ = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(1, 3, 2)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(1, this->state_, this->collisionShape_, fallInertia);
	this->fallRigidBody_ = new btRigidBody(fallRigidBodyCI);
	this->fallRigidBody_->setLinearVelocity(btVector3(1.1, 2.0, 0.0));

	this->positionID_ = std::string("transform");
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