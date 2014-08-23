#include <rapidXML/rapidxml.hpp>
#include <State.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Darknec {
	namespace ComponentRWUtils {

		float readFloat(rapidxml::xml_node<>* rootNode, const char* name);
		int readInt(rapidxml::xml_node<>* rootNode, const char* name);

		glm::vec2 readVector2(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y);
		glm::vec3 readVector3(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z);
		glm::vec4 readVector4(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z, const char* w);
		glm::fquat readQuat(rapidxml::xml_node<>* rootNode, const char* name, const char* x, const char* y, const char* z, const char* w);

	}
}