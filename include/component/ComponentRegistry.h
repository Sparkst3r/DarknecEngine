#ifndef DARKNEC_COMPONENTABSTRACTFACTORY_H 
#define DARKNEC_COMPONENTABSTRACTFACTORY_H

#include <hash_map>

class GameObject;
class Component;

#include <component/Component.h>

class ComponentFactory;
#define DECL_COMPONENT_FACTORY(componentClass)\
	class ComponentFactoryDefault : public ComponentFactory {\
	public:\
		ComponentFactoryDefault() {}\
		virtual ~ComponentFactoryDefault() {}\
		virtual Component* createComponent(GameObject* container) {\
			return new componentClass(container); \
		}\
	};

/**
* ComponentFactory
*
* @class ComponentFactory ComponentRegistry.h "include/object/ComponentRegistry.h"
* @brief Constructs a Component.
* 
* Contructable Components contain a customised ComponentFactory to produce a Component of that type.
*
*/
class ComponentFactory {
public:

	/**
	* createComponent
	* @brief Constructs a component
	*
	* @param container the GameObject
	* @return Component* the constructed component.
	*/
	virtual Component* createComponent(GameObject* container) = 0;
};

/**
* ComponentFactory
*
* @class ComponentRegistry ComponentRegistry.h "include/object/ComponentRegistry.h"
* @brief Hashes a Component name with a constructable Component.
*
* Can create a Component from a Component name. All returned Components are cast down to the base class "Component"
*
*/
class ComponentRegistry {
public:

	/**
	* Default constructor. Registers default components.

	*/
	ComponentRegistry();

	/**
	* registerComponent
	*
	* @param componentTypeName the component name to hash the Component against.
	* @param factory the ComponentFactory that constructs the Component
	*/
	void registerComponent(std::string componentTypeName, ComponentFactory* factory);

	/**
	* removeComponent
	*
	* @param componentTypeName the component name to remove from the hashmap
	*/
	void removeComponent(std::string componentTypeName);


	/**
	* createComponent
	*
	* @param componentTypeName the name of the Component to construct.
	* @param factory the GameObject container of the Component. Components have to have an owner.
	*/
	Component* createComponent(std::string componentTypeName, GameObject* container);

private:

	/** HashMap of the component names to their ComponentFactorys */
	std::hash_map<std::string, ComponentFactory*> factories;
};
#endif // !DARKNEC_COMPONENTABSTRACTFACTORY_H
