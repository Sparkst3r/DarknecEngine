#ifndef DARKNEC_COMPONENTPHYSICS_H 
#define DARKNEC_COMPONENTPHYSICS_H

#include <component/Component.h>
#include <component/ComponentTransform.h>
#include <physics/BulletTypeDef.h>

class ComponentPhysics : public Component {
public:
	DECL_COMPONENT_FACTORY(ComponentPhysics)

	ComponentPhysics(GameObject* container);

	void setPositionID(std::string posID) {
		this->positionID_ = posID;
	}

	std::string positionID_;

	CollisionShape* collisionShape_;
	DMotionState* state_;
	btRigidBody* fallRigidBody_;

	virtual void update();
	virtual void read(rapidxml::xml_node<>* node);

	virtual rapidxml::xml_node<>* write(rapidxml::xml_node<>*) {
		return NULL;
	}

};

#endif // !DARKNEC_COMPONENTPHYSICS_H
