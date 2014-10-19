#ifndef DARKNEC_WORLD_H
#define DARKNEC_WORLD_H

#include <State.h>
#include <physics/BulletTypeDef.h>
#include <object/GameObject.h>
#include <component/ComponentPhysics.h>
#include <component/ComponentMesh.h>

class World {
public:
	DiscDynWorld* physicsWorld;
	World();
	~World();


	void spawnObject(GameObject* object, glm::vec3 coordinates);


	void spawnObject(GameObject* object, glm::vec3 coordinates, std::string positionID, std::string physicsID);


	void renderAllObjects();

	void setGravity(btVector3 &gravityVector);

	void stepPhysicsWorld(btScalar timeStep, int maxSubSteps);


private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	std::vector<GameObject*> gameObjects;

};

#endif
