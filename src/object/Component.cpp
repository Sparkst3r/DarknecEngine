#include <object/Component.h>

Component::~Component() {}

void Component::setLogger(Logger* logger) {
	this->componentLogger = logger;
}