#include <object/World.h>


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
	Component* compTrans = object->getComponent(positionID);
	ComponentTransform* trans = dynamic_cast<ComponentTransform*>(compTrans);
	if (trans != NULL) {
		trans->setPosition(coordinates);
	}

	Component* compPhys = object->getComponent(physicsID);
	ComponentPhysics* phys = dynamic_cast<ComponentPhysics*>(compPhys);
	if (phys != NULL) {
		physicsWorld->addRigidBody(phys->fallRigidBody_);
	}
	gameObjects.push_back(object);
}

void World::renderAllObjects() {

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
		Component* compPhys = object->getComponent("physics"); //TODO dont hard code id. Fix when data driven files are added
		ComponentPhysics* phys = dynamic_cast<ComponentPhysics*>(compPhys);
		if (phys != NULL) {
			phys->update();
		}
	}

	for (Object* object : objects) {
		object->updatePosition();
	}
}
