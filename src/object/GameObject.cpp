#include <object/GameObject.h>
#include <object/ObjectFileParser.h>

GameObject::GameObject() {

}

GameObject* GameObject::createObject(const char* file) {
	return Darknec::ObjectFileParser::readFile(file);
}


GameObject::~GameObject() {
	for (std::pair<std::string, Component*> comp : components) {
		delete comp.second;
	}
}

Component* GameObject::getComponent(std::string ID) {
	if (components[ID] != NULL) {
		return components[ID];
	}
	else {
		Darknec::logger("GameObject", LogLevel::LOG_ERROR, "No component with ID: %s", ID.c_str());
		return NULL;
	}
}



std::string GameObject::registerComponent(Component* component, std::string ID) {
	components[ID] = component;
	return ID;
}

void GameObject::setName(std::string name) {
	this->name = name;
}
std::string GameObject::getName() const {
	return this->name;
}

