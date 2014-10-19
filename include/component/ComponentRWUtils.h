#ifndef DARKNEC_COMPONENTRWUTILS_H 
#define DARKNEC_COMPONENTRWUTILS_H

#include <State.h>

namespace Darknec {
	namespace ComponentRWUtils {


		////template <typename ComponentType>
		//void* readRequirement(rapidxml::xml_node<>* rootNode, std::string type, std::string ID, GameObject* container) {
		//	//for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		//	//	if (std::string(dataIter->name()) == std::string("ComponentRequirement")) {
		//	//		rapidxml::xml_attribute<>* attr1 = dataIter->first_attribute();
		//	//		rapidxml::xml_attribute<>* attr2 = attr1->next_attribute();
		//	//		if (std::string(attr1->name()) == std::string("id") && std::string(attr2->name()) == std::string("type")) { //TODO Make independant of attr order
		//	//			std::string id = std::string(attr1->value());
		//	//			if (id == ID) {
		//	//				std::string type = std::string(attr2->value());

		//	//				ComponentRequirementFactory* factory = Darknec::componentFactory->getComponentRequirementFactoryHash()[type];
		//	//				if (factory != NULL) {
		//	//					return factory->createComponentRequirement(container, std::string(dataIter->value()));
		//	//				}
		//	//				else {
		//	//					Darknec::logger(LogLevel::LOG_ERROR, "ComponentRequirement of type '%s' requested by Component does not exist in component registry", type.c_str());
		//	//					return NULL;
		//	//				}
		//	//			}
		//	//		}
		//	//	}
		//	//}
		//	//Darknec::logger(LogLevel::LOG_WARN, "Could not find ComponentRequirement with id:", ID.c_str());
		//	return NULL;
		//}

		std::string readString(rapidxml::xml_node<>* rootNode, const char* name);
		float readFloat(rapidxml::xml_node<>* rootNode, const char* name);
		int readInt(rapidxml::xml_node<>* rootNode, const char* name);

		glm::vec2 readVector2(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y);
		glm::vec3 readVector3(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z);
		glm::vec4 readVector4(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z, const char* w);
		glm::fquat readQuat(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z, const char* w);

	}
}
#endif // !DARKNEC_COMPONENTRWUTILS_H
