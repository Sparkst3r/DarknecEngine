#ifndef DARKNEC_COMPONENTTRANSFORM_H
#define DARKNEC_COMPONENTTRANSFORM_H

#include <object/Component.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class ComponentTransform : public Component {
public:
	ComponentTransform(GameObject* container);

	virtual void update() {}

	glm::vec3 getPosition() const;
	glm::fquat getOrientation() const;
	glm::vec3 getScale() const;

	void setPosition(glm::vec3 position);
	void setOrientation(glm::fquat orient);
	void setScale(glm::vec3 scale);

	void offsetPosition(glm::vec3 positionOffset);
	void offsetScale(glm::vec3 scaleOffset);

private:

	glm::vec3 position_;
	glm::fquat orient_;
	glm::vec3 scale_;

};
#endif // !DARKNEC_COMPONENTTRANSFORM_H
