#include <object/World.h>
#include <component/ComponentSimpleText.h>

World::World() {
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	physicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	

}

World::~World() {
	delete physicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}

void World::spawnObject(GameObject* object, glm::vec3 coordinates, std::string positionID, std::string physicsID) {

	if (object->hasComponent(physicsID)) {
		ComponentPhysics* phys = object->getCastComponent<ComponentPhysics>(physicsID);
		if (phys != NULL) {
			physicsWorld->addRigidBody(phys->rigidBody_);
		}
	}


	gameObjects.push_back(object);
}

void World::spawnObject(GameObject* object, glm::vec3 coordinates) {
	gameObjects.push_back(object);
}



void World::renderAllObjects() {

	for (GameObject* object : gameObjects) {

		if (object->hasComponent("mesh")) {
			ComponentMesh* mesh = object->getCastComponent<ComponentMesh>("mesh");
			mesh->update();
		}

		if (object->hasComponent("text")) {
			ComponentSimpleText* mesh = object->getCastComponent<ComponentSimpleText>("text");
			mesh->update();
		}

	}
}

void World::stepPhysicsWorld(btScalar timeStep, int maxSubSteps) {
	this->physicsWorld->stepSimulation(timeStep, maxSubSteps);


	for (GameObject* object : gameObjects) {
		if (object->hasComponent(std::string("physics"))) {
			ComponentPhysics* phys = object->getCastComponent<ComponentPhysics>(std::string("physics"));
			phys->update();
		}
	}
}
