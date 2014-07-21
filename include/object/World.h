#ifndef DARKNEC_WORLD_H
#define DARKNEC_WORLD_H

#include <vector>
#include <physics/BulletTypeDef.h>
#include <object/GameObject.h>
#include <object/Object.h>
#include <object/ObjectCamera.h>
#include <object/ComponentTransform.h>
#include <object/ComponentPhysics.h>
#include <physics/IKinematicPhysicsObject.h>
#include <object/ObjectMesh.h>
#include <hash_map>

class World {
public:
	DiscDynWorld* physicsWorld;
	World();
	~World();

	void spawnObject(Object* object, glm::vec3 coordinates);

	void spawnObject(GameObject* object, glm::vec3 coordinates, const char* positionID, const char* physicsID);


	void renderAllObjects();

	void setGravity(btVector3 &gravityVector);

	void stepPhysicsWorld(btScalar timeStep, btScalar fixedTimeStep);


private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;


	std::vector<Object*> objects;

	std::vector<GameObject*> gameObjects;

};

#endif
