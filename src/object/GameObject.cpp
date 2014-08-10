#include <object/GameObject.h>
#include <object/ObjectFileParser.h>

GameObject::GameObject() {

}

GameObject* GameObject::createObject(const char* file) {
	return Darknec::ObjectFileParser::readFile(file);
}


GameObject::~GameObject() {
	for (std::pair<const char*, Component*> comp : components) {
		delete comp.second;
	}
}

Component* GameObject::getComponent(const char* ID) {
	std::hash_map<const char*, Component*>::iterator pair = components.find(ID);
	
	if (pair != components.end()) {
		return pair->second;
	}
	else {
		Darknec::logger("GameObject", LogLevel::LOG_WARN, "No component with ID: %s", ID);
		return NULL;
	}
}



const char* GameObject::registerComponent(Component* component, const char* ID) {

	components[ID] = component;
	return ID;
}

void GameObject::setName(const char* name) {
	this->name = name;
}
const char* GameObject::getName() const {
	return this->name;
}

