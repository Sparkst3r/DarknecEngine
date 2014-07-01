#ifndef DARKNEC_IKINEMATICPHYSICSOBJECT_H
#define DARKNEC_IKINEMATICPHYSICSOBJECT_H
#include <physics/BulletTypeDef.h>

class IKinematicPhysicsObject {
public:

	virtual DMotionState* getMotionState() = 0;

	virtual btRigidBody* getRigidBody() = 0;


protected:
	CollisionShape* collisionShape;
	DMotionState* state;
	btRigidBody* fallRigidBody;



};

#endif