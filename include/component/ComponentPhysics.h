#ifndef DARKNEC_COMPONENTPHYSICS_H 
#define DARKNEC_COMPONENTPHYSICS_H

#include <State.h>
#include <component/Component.h>
#include <component/ComponentTransform.h>
#include <component/ComponentRWUtils.h>
#include <physics/BulletTypeDef.h>

class ComponentPhysics : public Component {
public:
	DECL_COMPONENT_FACTORY(ComponentPhysics)
	DECL_COMPONENTREQUIREMENT_FACTORY(ComponentPhysics)

	ComponentPhysics(GameObject* container);

	ComponentRequirement<ComponentTransform> transform_;

	CollisionShape* collisionShape_;
	DMotionState* state_;
	btRigidBody* rigidBody_;

	/** Temp hack */
	bool isStatic = false;

	void setInitialTransform(ComponentTransform* trans);

	virtual void update();
	virtual void read(XMLNode node);
	virtual void init();

	virtual bool validate();

	virtual XMLNode write(XMLNode node) {
		return NULL;
	}

};

#endif // !DARKNEC_COMPONENTPHYSICS_H
