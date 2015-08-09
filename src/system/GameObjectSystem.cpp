#include <system/GameobjectSystem.h>
#include <system/ComponentSystem.h>


GameObject* GameObjectSystem::makeObject(std::string objectName) {
	std::string file = this->registry[objectName];
	if (file != std::string("") && !file.empty()) {
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

			std::vector<Component*> loadedComponents;

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
									component->setName(compName);
									component->read(componentIter);
									
									loadedComponents.push_back(component);
								}
								else {
									Darknec::logger(Darknec::LogLevel::LOG_WARN, "Component of type '%s' requested by GameObject data file '%s' does not exist in component registry", s.c_str(), file);
								}
							}
						}
					}
				}
			}

			std::vector<Component*> bufferComponents;

			bufferComponents = loadedComponents;

			bool correctOrder = false;

			while (!correctOrder) {
				std::vector<Component*> bufferNotValidComponents;
				for (Component* comp : bufferComponents) {
					if (comp->validate()) {
						object->registerComponent(comp, comp->getName());
					}
					else {
						bufferNotValidComponents.push_back(comp);
					}
				}
				if (bufferNotValidComponents.empty()) {
					correctOrder = true;
				}
				bufferComponents = bufferNotValidComponents;
			}
/*
			for (Component* comp : loadedComponents) {
				object->registerComponent(comp, comp->getName());
			}*/

			std::hash_map<std::string, Component*> hash = object->getComponentMap();

			typedef std::hash_map<std::string, Component*>::iterator iter;
			for (iter i = hash.begin(); i != hash.end(); i++) {
				i->second->init();
			}


			return object;
		}
		return NULL;
	}
}

void GameObjectSystem::registerObject(std::string objectName, std::string objectXMLFile) {
	this->registry[objectName] = Darknec::baseAssetPath + objectXMLFile;
}

void GameObjectSystem::removeObject(std::string objectName) {
	std::hash_map<std::string, std::string>::iterator pair = this->registry.find(objectName);
	registry.erase(pair);
}

GameObjectSystem* sys2 = new GameObjectSystem();