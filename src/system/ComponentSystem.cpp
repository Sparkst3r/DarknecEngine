#include <system/ComponentSystem.h>
#include <component/ComponentTransform.h>
#include <component/ComponentCamera.h>
#include <component/ComponentMesh.h>
#include <component/ComponentPhysics.h>

ComponentSystem::ComponentSystem() {
	this->registerComponent("ComponentTransform", new ComponentTransform::ComponentFactoryDefault(), new ComponentTransform::ComponentRequirementFactoryDefault());
	this->registerComponent("ComponentCamera", new ComponentCamera::ComponentFactoryDefault(), new ComponentCamera::ComponentRequirementFactoryDefault());
	this->registerComponent("ComponentMesh", new ComponentMesh::ComponentFactoryDefault(), new ComponentMesh::ComponentRequirementFactoryDefault());
	this->registerComponent("ComponentPhysics", new ComponentPhysics::ComponentFactoryDefault(), new ComponentPhysics::ComponentRequirementFactoryDefault());
}

std::hash_map<std::string, ComponentFactory*> ComponentSystem::getComponentFactoryHash() const {
	return this->factories;
}

std::hash_map<std::string, ComponentRequirementFactory*> ComponentSystem::getComponentRequirementFactoryHash() const {
	return this->requirementFactories;
}



/**
* registerComponent
*
* @param componentTypeName the component name to hash the Component against.
* @param factory the ComponentFactory that constructs the Component
*/
void ComponentSystem::registerComponent(std::string componentTypeName, ComponentFactory* factory, ComponentRequirementFactory* reqFactory) {
	factories[componentTypeName] = factory;
	requirementFactories[componentTypeName] = reqFactory;
}

void* ComponentSystem::createRequirement(std::string componentTypeName, GameObject* container, std::string componentName) {
	ComponentRequirementFactory* factory = requirementFactories[componentTypeName];
	if (factory != NULL) {
		return factory->createComponentRequirement(container, componentName);
	}
	return NULL;
}


/**
* createComponent
*
* @param componentTypeName the name of the Component to construct.
* @param factory the GameObject container of the Component. Components have to have an owner.
*/
void ComponentSystem::removeComponent(std::string componentTypeName) {
	std::hash_map<std::string, ComponentFactory*>::iterator pair = factories.find(componentTypeName);

	delete pair->second;
	factories.erase(pair);
}

Component* ComponentSystem::createComponent(std::string componentTypeName, GameObject* container) {
	ComponentFactory* factory = factories[componentTypeName];
	if (factory != NULL) {
		return factory->createComponent(container);
	}
	return NULL;
}


ComponentSystem* sys1 = new ComponentSystem();