#ifndef DARKNEC_COMPONENT_H
#define DARKNEC_COMPONENT_H

#include <State.h>
#include <object/GameObject.h>


class Component {
public:
	virtual ~Component() {};

	virtual void update() = 0;

protected:
	GameObject* container_;

};

#endif // !DARKNEC_COMPONENT_H
