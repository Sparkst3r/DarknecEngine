#include <object/GameObject.h>

#include <component/Component.h>

GameObject::GameObject() {

}

GameObject* GameObject::readFile(const char* file) {
	std::ifstream fileStream(file);

	if (fileStream.good()) {
		GameObject* object = new GameObject();

		rapidxml::xml_document<> doc;
		rapidxml::xml_node<>* root_node;

		std::stringstream stringStream;
		stringStream << fileStream.rdbuf();
		std::string xmlDataString = stringStream.str();

		char* cstr = new char[xmlDataString.size() + 1];
		strcpy_s(cstr, xmlDataString.size() + 1, xmlDataString.c_str());

		doc.parse<0>(cstr);

		root_node = doc.first_node("GameObject"); //Root node

		for (rapidxml::xml_node<>* gameObjectIter = root_node->first_node(); gameObjectIter; gameObjectIter = gameObjectIter->next_sibling()) {

			if (std::string(gameObjectIter->name()) == std::string("name")) {
				object->setName(gameObjectIter->value());
			}
			else if (std::string(gameObjectIter->name()) == std::string("components")) {
				for (rapidxml::xml_node<>* componentIter = gameObjectIter->first_node(); componentIter; componentIter = componentIter->next_sibling()) {
					if (std::string(componentIter->name()) == std::string("component")) {

						rapidxml::xml_attribute<>* attrType = componentIter->first_attribute();
						rapidxml::xml_attribute<>* attrName = attrType->next_attribute();

						if (std::string(attrType->name()) == std::string("type") && std::string(attrName->name()) == std::string("name")) {
							std::string s = std::string(attrType->value());
							Component* component = sys1->createComponent(s, object);
							if (component != NULL) {
								const char* charw = reinterpret_cast<const char*>(attrName->value());

								std::string compName = std::string(charw);
								compName = charw;

								component->read(componentIter);
								object->registerComponent(component, compName.c_str());
							}
							else {
								Darknec::logger(Darknec::LogLevel::LOG_WARN, "Component of type '%s' requested by GameObject data file '%s' does not exist in component registry", s.c_str(), file);
							}
						}
					}
				}
			}
		}

		

		return object;
	}
	return NULL;
}


GameObject::~GameObject() {
	for (std::pair<std::string, Component*> comp : components_) {
		delete comp.second;
		comp.second = NULL;
	}
}

Component* GameObject::getComponent(std::string ID) {
	if (components_[ID] != NULL) {
		return components_[ID];
	}
	else {
		Darknec::logger("GameObject", Darknec::LogLevel::LOG_ERROR, "No component with ID: %s", ID.c_str());
		return NULL;
	}
}

bool GameObject::hasComponent(std::string ID) {
	return components_[ID] != NULL;
}



std::string GameObject::registerComponent(Component* component, std::string ID) {
	components_[ID] = component;
	return ID;
}

void GameObject::setName(std::string name) {
	this->name = name;
}
std::string GameObject::getName() const {
	return this->name;
}

