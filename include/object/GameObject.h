#ifndef DARKNEC_GAMEOBJECT_H
#define DARKNEC_GAMEOBJECT_H

#include <State.h>

#include <hash_map>
class Component;

/**
* GameObject
*
* @class GameObject GameObject.h "include/object/GameObject.h"
* @brief Core game object
*
* Base object for everything in a game.
* Container for Components. Components provide the functionality of the GameObject shell.
*
*/
class GameObject {

public:

	GameObject();

	static GameObject* createObject(const char* file);

	/**
	* Destructor
	* Performs iteration over Component map and deletes components
	*/
	~GameObject();

	/**
	* getComponent
	*
	* Gets a registered object component from the gameobject as a polymorphic component
	*
	* @param ID component ID
	* @return pointer to the component
	*/
	Component* getComponent(std::string ID);

	/**
	* getCastComponent
	*
	* Gets a registered object component from the gameobject as a cast component
	*
	* @tparam T Component type to cast to 
	* @param ID component ID
	* @return pointer to the cast component
	*/
	template <typename T>
	T* getCastComponent(std::string ID) {
		return dynamic_cast<T*>(getComponent(ID));
	}
	

	std::string registerComponent(Component* component, std::string ID);
	
	void setName(std::string name);
	std::string getName() const;

	std::hash_map<std::string, Component*> getComponentMap() const {
		return components;
	}


private:

	std::string name;

	std::hash_map<std::string, Component*> components;

};

#endif // !DARKNEC_GAMEOBJECT_H