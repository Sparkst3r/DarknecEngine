#ifndef DARKNEC_COMPONENTRWUTILS_H 
#define DARKNEC_COMPONENTRWUTILS_H

#include <State.h>
#include <system/ComponentSystem.h>



namespace Darknec {
	namespace ComponentRWUtils {


		template <typename ComponentType>
		ComponentRequirement<ComponentType> readRequirement(rapidxml::xml_node<>* rootNode, std::string type, std::string name, GameObject* container) {
			for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
				if (std::string(dataIter->name()) == std::string("ComponentRequirement")) {
					rapidxml::xml_attribute<>* typeAttr = dataIter->first_attribute();
					rapidxml::xml_attribute<>* nameAttr = typeAttr->next_attribute();
					if (std::string(typeAttr->name()) == std::string("type") && std::string(nameAttr->name()) == std::string("name")) { //TODO Make independant of attr order
						std::string nameVal = std::string(nameAttr->value());
						if (nameVal == name) {
							std::string typeVal = std::string(typeAttr->value());

							ComponentRequirementFactory* factory = sys1->getComponentRequirementFactoryHash()[typeVal];
							if (factory != NULL) {
								void* req = factory->createComponentRequirement(container, std::string(dataIter->value()));
								
								ComponentRequirement<ComponentType>* reqCast = static_cast<ComponentRequirement<ComponentType>*>(req);
								ComponentRequirement<ComponentType> reqObj = *reqCast;
								delete reqCast;
								return reqObj;
							}
							else {
								Darknec::logger(LogLevel::LOG_ERROR, "ComponentRequirement of type '%s' requested by Component does not exist in component registry", typeVal.c_str());
								
								ComponentRequirement<ComponentType> reqBackup = ComponentRequirement<ComponentType>(container, std::string(dataIter->value()));
								
								return reqBackup;
							}
						}
					}
				}
			}
			Darknec::logger(LogLevel::LOG_WARN, "Could not find ComponentRequirement with id:", name.c_str());
		}

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
