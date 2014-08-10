#include <object/ObjectFileParser.h>

#include <component/ComponentRegistry.h>

namespace Darknec {

	namespace ObjectFileParser {
		using namespace rapidxml;

		GameObject* readFile(const char* file) {
			std::ifstream fileStream(file);

			if (fileStream.good()) {
				GameObject* object = new GameObject();

				xml_document<> doc;
				xml_node<>* root_node;

				std::stringstream stringStream;
				stringStream << fileStream.rdbuf();
				std::string xmlDataString = stringStream.str();
				std::vector<char> data(xmlDataString.begin(), xmlDataString.end());
				data.push_back('\0');
				doc.parse<0>(&data[0]);

				root_node = doc.first_node("GameObject"); //Root node

				for (xml_node<>* gameObjectIter = root_node->first_node(); gameObjectIter; gameObjectIter = gameObjectIter->next_sibling()) {
					
					if (std::string(gameObjectIter->name()) == std::string("name")) {
						object->setName(gameObjectIter->value());
					}
					else if (std::string(gameObjectIter->name()) == std::string("components")) {
						for (xml_node<>* componentIter = gameObjectIter->first_node(); componentIter; componentIter = componentIter->next_sibling()) {
							if (std::string(componentIter->name()) == std::string("component")) {

								xml_attribute<char>* attrType = componentIter->first_attribute();
								xml_attribute<char>* attrName = attrType->next_attribute();
								
								if (std::string(attrType->name()) == std::string("type") && std::string(attrName->name()) == std::string("name")) {
									std::string s = std::string(attrType->value());
									Component* component = Darknec::componentFactory->createComponent(s, object);
									if (component != NULL) {
										std::string compName = std::string(attrName->value());

										component->read(componentIter);
										object->registerComponent(component, compName.c_str());
									}
									else {
										Darknec::logger(LogLevel::LOG_WARN, "Component of type '%s' requested by GameObject data file '%s' does not exist in component registry", s.c_str(), file);
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
	}
}