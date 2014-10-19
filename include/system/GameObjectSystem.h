#ifndef DARKNEC_GAMEOBJECTSYSTEM_H 
#define DARKNEC_GAMEOBJECTSYSTEM_H

#include <State.h>
#include <object/GameObject.h>


class GameObjectSystem {
public:
	GameObject* makeObject(std::string objectName);

	void registerObject(std::string objectName, std::string objectXMLFile);

	void removeObject(std::string objectName);

private:

	std::hash_map<std::string, std::string> registry;
};

extern GameObjectSystem* sys2;

#endif // !DARKNEC_GAMEOBJECTSYSTEM_H
