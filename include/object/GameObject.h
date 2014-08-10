#ifndef DARKNEC_GAMEOBJECT_H
#define DARKNEC_GAMEOBJECT_H

#include <vector>
#include <hash_map>
#include <State.h>

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
	Component* getComponent(const char* ID);

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
	T* getCastComponent(const char* ID) {
		return dynamic_cast<T*>(getComponent(ID));
	}
	

	const char* registerComponent(Component* component, const char* ID);
	
	void setName(const char* name);
	const char* getName() const;


private:

	const char* name;

	std::hash_map<const char*, Component*> components;

};

#endif // !DARKNEC_GAMEOBJECT_H