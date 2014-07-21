#include <object/GameObject.h>


GameObject::~GameObject() {
	for (std::pair<const char*, Component*> comp : components) {
		delete comp.second;
	}
}

Component* GameObject::getComponent(const char* ID) {
	std::hash_map<const char*, Component*>::iterator map = components.find(ID);
	
	if (map != components.end()) {
		return map->second;
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

