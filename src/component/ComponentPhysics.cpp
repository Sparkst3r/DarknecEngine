#include <component/ComponentPhysics.h>

ComponentPhysics::ComponentPhysics(GameObject* container) {
	container_ = container;
}

void ComponentPhysics::setInitialTransform(ComponentTransform* trans) {
	glm::fquat q = trans->getOrientation();
	glm::vec3 v = trans->getPosition();
	btVector3 transform = btVector3(v.x, v.y, v.z);
	btQuaternion quat = btQuaternion(q.x, q.y, q.z, q.w);


	btTransform tr;
	this->rigidBody_->getMotionState()->getWorldTransform(tr);
	tr.setOrigin(transform);
	tr.setRotation(quat);
	this->rigidBody_->getMotionState()->setWorldTransform(tr);



}

void ComponentPhysics::init() {
	this->transform_.setup();
}

bool ComponentPhysics::validate() {
	return this->transform_.validate() && this->transform_->validate();
}

void ComponentPhysics::read(XMLNode node) {
	this->transform_ = Darknec::ComponentRWUtils::readRequirement<ComponentTransform>(node, std::string("ComponentTransform"), std::string("transform"), this->container_);

	Darknec::logger(this->transform_.name.c_str());

	btVector3 velocity;

	for (rapidxml::xml_node<>* dataIter = node->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		if (std::string(dataIter->name()) == std::string("CollisionType")) {
			std::string type = Darknec::ComponentRWUtils::readString(dataIter, "Type");

			if (type == std::string("SPHERE")) {
				float size = Darknec::ComponentRWUtils::readFloat(dataIter, "Size");
				
				glm::vec3 v = Darknec::ComponentRWUtils::readVector3(dataIter, "Velocity", "X", "Y", "Z");
				velocity = btVector3(v.x, v.y, v.z);

				if (size == NULL) {
					Darknec::logger("SPHERE CollisionType missing 'Size' tag");
					size = 1.0f;
				}
				this->collisionShape_ = new btSphereShape(size);
			}
			else {
				if (type == std::string("STATICPLANE")) {
			
					this->isStatic = true;
					glm::vec3 v = Darknec::ComponentRWUtils::readVector3(dataIter, "PlaneVector", "X", "Y", "Z");
					float planeConstant = Darknec::ComponentRWUtils::readFloat(dataIter, "PlaneConstant");
					btVector3 vectorbt = btVector3(v.x, v.y, v.z);

					Darknec::logger("sss");
					this->collisionShape_ = new btStaticPlaneShape(vectorbt, planeConstant);
				}
			}
		}
	}

	glm::vec3 p = glm::vec3(0,0,0);
	glm::fquat q = glm::fquat(0,0,0,1);

	btVector3 fallInertia(0, 0, 0);
	this->collisionShape_->calculateLocalInertia(1, fallInertia);
	this->state_ = new btDefaultMotionState(btTransform(btQuaternion(q.x, q.y, q.z, q.w), btVector3(p.x, p.y, p.z)));
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(1, this->state_, this->collisionShape_, fallInertia);
	this->rigidBody_ = new btRigidBody(fallRigidBodyCI);
	this->rigidBody_->setLinearVelocity(velocity);

}

bool b3 = true;

void ComponentPhysics::update() {
	if (b3) {
		b3 = false;
		setInitialTransform(this->transform_.getComponent());
	}

	if (!isStatic) {

		btTransform trans;
		rigidBody_->getMotionState()->getWorldTransform(trans);
		btVector3 p = trans.getOrigin();
		btQuaternion o = trans.getRotation();
		transform_->setPosition(glm::vec3(p.getX(), p.getY(), p.getZ()));
		transform_->setOrientation(glm::fquat(o.getX(), o.getY(), o.getZ(), o.getW()));
	}

}