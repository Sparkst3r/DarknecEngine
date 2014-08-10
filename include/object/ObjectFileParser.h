#ifndef DARKNEC_OBJECTFILEPARSER_H 
#define DARKNEC_OBJECTFILEPARSER_H

#include <State.h>
#include <object/GameObject.h>

#include <rapidxml/rapidxml.hpp>



namespace Darknec {

	namespace ObjectFileParser {

		GameObject* readFile(const char* file);

	}

}
#endif // !DARKNEC_OBJECTFILEPARSER_H
