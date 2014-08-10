#include <component/ComponentRegistry.h>

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
