#ifndef DARKNEC_GAMEOBJECT_H
#define DARKNEC_GAMEOBJECT_H

#include <vector>
#include <hash_map>
#include <State.h>

class Component;

/**
* @class GameObject GameObject.h "include/object/GameObject.h"
* @brief Core game object
*
* Base object for everything in a game
*
*/
class GameObject {

public:

	/**
	* Destructor
	* Performs iteration over Component map and deletes components
	*/
	~GameObject();

	Component* getComponent(const char* ID);

	template <typename T>
	T* getCastComponent(const char* ID) {
		return dynamic_cast<T*>(getComponent(ID));
	}
	

	const char* registerComponent(Component* component, const char* ID);
	



private:

	const char* name;

	std::hash_map<const char*, Component*> components;

};

#endif // !DARKNEC_GAMEOBJECT_H