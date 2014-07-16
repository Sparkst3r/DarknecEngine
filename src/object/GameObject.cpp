#include <object/GameObject.h>


GameObject::GameObject() {}

GameObject::~GameObject() {
	for (std::pair<const char*, Component*> comp : components) {
		delete comp.second;
	}
}

Component* GameObject::getComponent(const char* ID) {
	auto map = components.find(ID);
	return map->second;
}

const char* GameObject::registerComponent(Component* component, const char* ID) {

	components[ID] = component;
	return ID;
}