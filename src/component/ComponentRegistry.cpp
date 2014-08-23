#include <component/ComponentRegistry.h>

#include <component/ComponentTransform.h>
#include <component/ComponentPhysics.h>
#include <component/ComponentCamera.h>
#include <component/ComponentMesh.h>


ComponentRegistry::ComponentRegistry() {
	this->registerComponent(std::string("ComponentTransform"), new ComponentTransform::ComponentFactoryDefault());
	this->registerComponent(std::string("ComponentPhysics"), new ComponentPhysics::ComponentFactoryDefault());
	this->registerComponent(std::string("ComponentCamera"), new ComponentCamera::ComponentFactoryDefault());
	this->registerComponent(std::string("ComponentMesh"), new ComponentMesh::ComponentFactoryDefault());
}


/**
* registerComponent
*
* @param componentTypeName the component name to hash the Component against.
* @param factory the ComponentFactory that constructs the Component
*/
void ComponentRegistry::registerComponent(std::string componentTypeName, ComponentFactory* factory) {
	factories[componentTypeName] = factory;
}

/**
* createComponent
*
* @param componentTypeName the name of the Component to construct.
* @param factory the GameObject container of the Component. Components have to have an owner.
*/
void ComponentRegistry::removeComponent(std::string componentTypeName) {
	std::hash_map<std::string, ComponentFactory*>::iterator pair = factories.find(componentTypeName);

	delete pair->second;
	factories.erase(pair);
}

Component* ComponentRegistry::createComponent(std::string componentTypeName, GameObject* container) {
	ComponentFactory* factory = factories[componentTypeName];
	if (factory != NULL) {
		return factory->createComponent(container);
	}
	return NULL;
}
