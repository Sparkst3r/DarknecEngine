#include <object/World.h>

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

void World::spawnObject(Object* object, glm::vec3 coordinates) {
	object->setPosition(coordinates);
	IKinematicPhysicsObject* cast = dynamic_cast<IKinematicPhysicsObject*>(object);
	if (cast) {
		physicsWorld->addRigidBody(cast->getRigidBody()); //If the object should interact with physics
	}
	objects.push_back(object);
}

void World::spawnObject(GameObject* object, glm::vec3 coordinates, const char* positionID, const char* physicsID) {
	ComponentTransform* trans = object->getCastComponent<ComponentTransform>(positionID);
	if (trans != NULL) {
		//trans->setPosition(coordinates);
	}

	ComponentPhysics* phys = object->getCastComponent<ComponentPhysics>(physicsID);
	if (phys != NULL) {
		physicsWorld->addRigidBody(phys->fallRigidBody_);
	}

	gameObjects.push_back(object);
}

void World::renderAllObjects() {

	for (GameObject* object : gameObjects) {
		ComponentMesh* mesh = object->getCastComponent<ComponentMesh>("mesh");
		if (mesh != NULL) {
			mesh->renderObject();
		}
	}
	


	for (Object* object : objects) {
		ObjectMesh* cast = dynamic_cast<ObjectMesh*>(object);
		if (cast) {
			cast->renderObject();
		}
	}
}

void World::stepPhysicsWorld(btScalar timeStep, int maxSubSteps) {
	this->physicsWorld->stepSimulation(timeStep, maxSubSteps);

	for (GameObject* object : gameObjects) {
		ComponentPhysics* phys = object->getCastComponent<ComponentPhysics>("physics");
		if (phys != NULL) {
			phys->update();
		}
	}
	

	for (Object* object : objects) {
		object->updatePosition();
	}
}
