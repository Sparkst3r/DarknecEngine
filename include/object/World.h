#ifndef DARKNEC_WORLD_H
#define DARKNEC_WORLD_H

#include <State.h>
#include <physics/BulletTypeDef.h>
#include <object/GameObject.h>
#include <component/ComponentPhysics.h>


class World {
public:
	DiscDynWorld* physicsWorld;
	World();
	~World();

	GameObject* getObject(std::string name);

	void spawnObject(std::string type, std::string ID);

	void renderAllObjects();

	void setGravity(btVector3 &gravityVector);

	void stepPhysicsWorld(btScalar timeStep, int maxSubSteps);

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	std::hash_map<std::string, GameObject*> gameObjects;

	GameObject* currentCamera;


};

#endif
