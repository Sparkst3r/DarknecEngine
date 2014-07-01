#ifndef DARKNEC_GAMEOBJECT_H
#define DARKNEC_GAMEOBJECT_H

#include <vector>
#include <hash_map>
#include <State.h>

class Component;

class GameObject {

public:
	Logger* componentLogger;

	GameObject();
	~GameObject();

	Component* getComponent(const char* ID);
	const char* registerComponent(Component* component, const char* ID);

private:

	const char* name;

	std::hash_map<const char*, Component*> components;

};

#endif // !DARKNEC_GAMEOBJECT_H