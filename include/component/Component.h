#ifndef DARKNEC_COMPONENT_H
#define DARKNEC_COMPONENT_H

#include <State.h>
#include <system/ComponentSystem.h>

#include <object/GameObject.h>

class Component {
public:
	virtual ~Component() {};

	virtual void update() = 0;

	virtual void read(rapidxml::xml_node<>* node) = 0;

	virtual rapidxml::xml_node<>* write(rapidxml::xml_node<>*) = 0;

	//virtual void validate() = 0;

	virtual std::string getName() const {
		return this->name_;
	}

	
protected:
	GameObject* container_;
	std::string name_;
};

#endif // !DARKNEC_COMPONENT_H
