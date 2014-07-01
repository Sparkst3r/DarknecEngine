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

void World::renderAllObjects() {

	for (Object* object : objects) {
		ObjectMesh* cast = dynamic_cast<ObjectMesh*>(object);
		if (cast) {
			cast->renderObject();
		}
	}
}

void World::requestUpdate() {
	for (Object* object : objects) {
		object->updatePosition();
	}
}
