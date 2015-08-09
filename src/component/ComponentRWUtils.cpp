#include <component/ComponentRWUtils.h>
#include <DarknecEngine.h>
namespace Darknec {
	namespace ComponentRWUtils {

		std::string readString(rapidxml::xml_node<>* rootNode, const char* name) {
			for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
				if (std::string(dataIter->name()) == std::string(name)) {
					return std::string(dataIter->value());
				}
			}
			Darknec::logger(LogLevel::LOG_WARN, "Could not read 'string' : %s", name);
			return std::string("");
		}

		float readFloat(rapidxml::xml_node<>* rootNode, const char* name) {
			for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
				if (std::string(dataIter->name()) == std::string(name)) {
					return (float) atof(std::string(dataIter->value()).c_str());
				}
			}
			Darknec::logger(LogLevel::LOG_WARN, "Could not read 'float' : %s", name);
			return NULL;
		}
		int readInt(rapidxml::xml_node<>* rootNode, const char* name) {
			for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
				if (std::string(dataIter->name()) == std::string(name)) {
					return atoi(std::string(dataIter->value()).c_str());
				}
			}
			Darknec::logger(LogLevel::LOG_WARN, "Could not read 'int' : %s", name);
			return NULL;
		}

		glm::vec2 readVector2(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y) {
			glm::vec2 vec;
			
			for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
				if (std::string(dataIter->name()) == std::string(name)) {
					rapidxml::xml_attribute<char>* orientComp = dataIter->first_attribute();
					std::string s = std::string(dataIter->value());
					if (std::string(orientComp->value()) == std::string(x)) {
						vec.x = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(y)) {
						vec.y = (float) atof(s.c_str());
					}
				}
			}
			return vec;
		}
		glm::vec3 readVector3(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z) {
			glm::vec3 vec;

			for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
				if (std::string(dataIter->name()) == std::string(name)) {
					rapidxml::xml_attribute<char>* orientComp = dataIter->first_attribute();
					std::string s = std::string(dataIter->value());
					if (std::string(orientComp->value()) == std::string(x)) {
						vec.x = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(y)) {
						vec.y = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(z)) {
						vec.z = (float) atof(s.c_str());
					}
				}

			}
			return vec;
		}
		glm::vec4 readVector4(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z, const char* w) {
			glm::vec4 vec;

			for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
				if (std::string(dataIter->name()) == std::string(name)) {
					rapidxml::xml_attribute<char>* orientComp = dataIter->first_attribute();
					std::string s = std::string(dataIter->value());
					if (std::string(orientComp->value()) == std::string(x)) {
						vec.x = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(y)) {
						vec.y = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(z)) {
						vec.z = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(w)) {
						vec.w = (float) atof(s.c_str());
					}
				}

			}
			return vec;
		}
		glm::fquat readQuat(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z, const char* w) {
			glm::fquat vec;

			for (rapidxml::xml_node<>* dataIter = rootNode->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
				if (std::string(dataIter->name()) == std::string(name)) {
					rapidxml::xml_attribute<char>* orientComp = dataIter->first_attribute();
					std::string s = std::string(dataIter->value());
					if (std::string(orientComp->value()) == std::string(x)) {
						vec.x = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(y)) {
						vec.y = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(z)) {
						vec.z = (float) atof(s.c_str());
					}
					else if (std::string(orientComp->value()) == std::string(w)) {
						vec.w = (float) atof(s.c_str());
					}
				}

			}
			return vec;
		}

	}
}