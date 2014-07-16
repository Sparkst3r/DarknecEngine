#ifndef DARKNEC_COMPONENT_H
#define DARKNEC_COMPONENT_H

#include <State.h>
#include <object/GameObject.h>


class Component {
public:
	virtual ~Component() = 0; //Dummy destructor

protected:
	GameObject* container_;

};

#endif // !DARKNEC_COMPONENT_H
