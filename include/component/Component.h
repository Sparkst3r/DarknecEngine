#ifndef DARKNEC_COMPONENT_H
#define DARKNEC_COMPONENT_H

#include <State.h>
#include <object/GameObject.h>
#include <rapidXML/rapidxml.hpp>
#include <component/ComponentRWUtils.h>

class Component {
public:
	virtual ~Component() {};

	virtual void update() = 0;

	virtual void read(rapidxml::xml_node<>* node) = 0;

	virtual rapidxml::xml_node<>* write(rapidxml::xml_node<>*) = 0;
	const char* name;
protected:
	GameObject* container_;

	

};

#endif // !DARKNEC_COMPONENT_H
