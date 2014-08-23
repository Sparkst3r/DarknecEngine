#include <system/GameobjectSystem.h>


GameObject* GameObjectSystem::makeObject(std::string objectName) {
	return NULL;
}

void GameObjectSystem::registerObject(std::string objectName, std::string objectXMLFile) {
	this->registry[objectName] = objectXMLFile;
}

void GameObjectSystem::removeObject(std::string objectName) {
	std::hash_map<std::string, std::string>::iterator pair = this->registry.find(objectName);
	registry.erase(pair);
}