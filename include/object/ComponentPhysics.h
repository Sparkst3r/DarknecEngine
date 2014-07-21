#ifndef DARKNEC_COMPONENTPHYSICS_H 
#define DARKNEC_COMPONENTPHYSICS_H

#include <object/Component.h>
#include <object/ComponentTransform.h>
#include <physics/BulletTypeDef.h>

class ComponentPhysics : public Component {
public:

	ComponentPhysics(GameObject* container, const char* positionID);

	const char* positionID_;

	CollisionShape* collisionShape_;
	DMotionState* state_;
	btRigidBody* fallRigidBody_;

	virtual void update();

};

#endif // !DARKNEC_COMPONENTPHYSICS_H
