#include <object/World.h>
#include <component/ComponentSimpleText.h>
#include <system/GameObjectSystem.h>
#include <component/ComponentMesh.h>

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

GameObject* World::getObject(std::string name) {
	return this->gameObjects[name];
}

void World::spawnObject(std::string type, std::string id) {
	GameObject* object = sys2->makeObject(type);

	if (object->hasComponent(std::string("physics"))) {
		ComponentPhysics* phys = object->getCastComponent<ComponentPhysics>(std::string("physics"));
		if (phys != NULL) {
			physicsWorld->addRigidBody(phys->rigidBody_);
		}
	}
	gameObjects[id] = object;
}


void World::renderAllObjects() {

	for (std::pair<std::string, GameObject*> obj : this->gameObjects) {
		GameObject* object = obj.second;
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


	for (std::pair<std::string, GameObject*> obj : this->gameObjects) {
		GameObject* object = obj.second;
		if (object->hasComponent(std::string("physics"))) {
			ComponentPhysics* phys = object->getCastComponent<ComponentPhysics>(std::string("physics"));
			phys->update();
		}
	}
}
