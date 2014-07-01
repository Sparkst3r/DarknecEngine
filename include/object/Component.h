#ifndef DARKNEC_COMPONENT_H
#define DARKNEC_COMPONENT_H

#include <State.h>
#include <object/GameObject.h>


class Component {
public:
	virtual ~Component() = 0; //Dummy destructor

	void setLogger(Logger* logger);

protected:
	GameObject* container_;
	Logger* componentLogger;

};

#endif // !DARKNEC_COMPONENT_H
