#ifndef DARKNEC_COMPONENTTRANSFORM_H
#define DARKNEC_COMPONENTTRANSFORM_H

#include <component/Component.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class ComponentTransform : public Component {
public:

	DECL_COMPONENT_FACTORY(ComponentTransform)

	ComponentTransform(GameObject* container);

	virtual void update() {}
	virtual void read(rapidxml::xml_node<>* node);

	virtual rapidxml::xml_node<>* write(rapidxml::xml_node<>*) {
		return NULL;
	}

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
