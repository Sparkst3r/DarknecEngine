#ifndef DARKNEC_COMPONENTSYSTEM_H 
#define DARKNEC_COMPONENTSYSTEM_H


#include <component/Component.h>
#include <object/GameObject.h>
#include <DarknecEngine.h>

/**
* ComponentRequirement
*
* Provides a proxy for accessing Components in the parent GameObject.
* Allows for ensuring a Component exists in the GameObject indirectly.
*
* @tparam T Component type to emulate
*/
template <class T>
class ComponentRequirement {
public:
	/**
	* Default constructor
	*/
	ComponentRequirement() {}

	/**
	* Constructor
	*
	*/
	ComponentRequirement(GameObject* container, std::string name) {
		this->exists_ = true;
		this->name = name;
		this->container_ = container;
		this->validate();
	}

	/**
	* Return direct pointer to the Component. Avoid if possible.
	* @return direct pointer.
	*/
	T* operator->() {
		if (exists_) {
			return this->component_;
		}
		Darknec::logger(Darknec::LogLevel::LOG_ERROR, "ComponentRequirement not initialised correctly: Cannot return component.");
		return NULL;
	}

	T* getComponent() const {
		if (exists_) {
			return this->component_;
		}
		Darknec::logger(Darknec::LogLevel::LOG_ERROR, "ComponentRequirement not initialised correctly: Cannot return component.");
		return NULL;
	}

	/**
	* Call to re validate the requirement if the container changes its Component list.
	* @return true if the requirement was still valid and no change was made
	*/
	bool validate() {
		if (this->exists_) {
			if (this->container_->hasComponent(name)) {
				T* component = this->container_->getCastComponent<T>(name);
				if (component != NULL) {
					this->component_ = component;
					return true;
				}
				Darknec::logger(Darknec::LogLevel::LOG_ERROR, "Component found '%s' is of incorrect Component type", name.c_str());
				return false;
			}
			Darknec::logger(Darknec::LogLevel::LOG_WARN, "Component doesnt exist with name : %s", name.c_str());
			this->container_->registerComponent(new T(this->container_), name);
			Darknec::logger(Darknec::LogLevel::LOG_INFO, "Inserted component for : %s", name.c_str());
			return false;
		}
		Darknec::logger(Darknec::LogLevel::LOG_ERROR, "ComponentRequirement not initialised correctly: Cannot validate.");
		return false;
	}



	std::string name;
private:
	bool exists_ = false;
	T* component_;
	GameObject* container_;

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
* ComponentRequirementFactory
*
* @class ComponentRequirementFactory ComponentSystem.h "include/system/ComponentSystem.h"
* @brief Constructs a Component.
*
* Contructable Components contain a customised ComponentFactory to produce a Component of that type.
*
*/
class ComponentRequirementFactory {
public:

	virtual void* createComponentRequirement(GameObject* container, std::string name) = 0;

};

#define DECL_COMPONENT_FACTORY(componentClass)\
class ComponentFactoryDefault : public ComponentFactory {\
public:\
	ComponentFactoryDefault() {}\
	virtual ~ComponentFactoryDefault() {}\
	virtual Component* createComponent(GameObject* container) {\
		return new componentClass(container); \
	}\
};

#define DECL_COMPONENTREQUIREMENT_FACTORY(componentClass)\
class ComponentRequirementFactoryDefault : public ComponentRequirementFactory {\
public:\
	ComponentRequirementFactoryDefault() {}\
	virtual ~ComponentRequirementFactoryDefault() {}\
	virtual void* createComponentRequirement(GameObject* container, std::string name) {\
		return new ComponentRequirement<componentClass>(container, name);\
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


/**
* ComponentSystem
*
* @class ComponentSystem ComponentSystem.h "include/system/ComponentSystem.h"
* @brief Hashes a Component name with a constructable Component.
*
* Can create a Component from a Component name. All returned Components are cast down to the base class "Component"
*
*/
class ComponentSystem {
public:

	/**
	* Default constructor. Registers default components.

	*/
	ComponentSystem();

	/**
	* registerComponent
	*
	* @param componentTypeName the component name to hash the Component against.
	* @param factory the ComponentFactory that constructs the Component
	* @param reqFactory the ComponentRequirementFactory that constructs a requirement for this component
	*/
	void registerComponent(std::string componentTypeName, ComponentFactory* factory, ComponentRequirementFactory* reqFactory);


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
	* @return the created Component
	*/
	Component* createComponent(std::string componentTypeName, GameObject* container);

	/**
	* createRequirement
	*
	* @param componentTypeName the name of the ComponentRequirement to construct.
	* @param factory the GameObject container of the Component. Components have to have an owner. 
	* @return type erased ComponentRequirement
	*/
	void* createRequirement(std::string componentTypeName, GameObject* container, std::string componentName);

	std::hash_map<std::string, ComponentFactory*> getComponentFactoryHash() const;

	std::hash_map<std::string, ComponentRequirementFactory*> getComponentRequirementFactoryHash() const;

private:

	/** HashMap of the component names to their ComponentFactorys */
	std::hash_map<std::string, ComponentFactory*> factories;

	std::hash_map<std::string, ComponentRequirementFactory*> requirementFactories;
};


extern ComponentSystem* sys1;




#endif // !DARKNEC_COMPONENTSYSTEM_H
