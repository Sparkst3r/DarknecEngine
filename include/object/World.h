#ifndef DARKNEC_WORLD_H
#define DARKNEC_WORLD_H

#include <vector>
#include <physics/BulletTypeDef.h>
#include <object/Object.h>
#include <object/ObjectCamera.h>
#include <physics/IKinematicPhysicsObject.h>
#include <object/ObjectMesh.h>
#include <hash_map>

class World {
public:
	DiscDynWorld* physicsWorld;
	World();
	~World();

	void spawnObject(Object* object, glm::vec3 coordinates);

	void renderAllObjects();

	void setGravity(btVector3 &gravityVector);

	void requestUpdate();


private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;


	std::vector<Object*> objects;

};

#endif
