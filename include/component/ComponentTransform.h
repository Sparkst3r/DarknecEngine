#ifndef DARKNEC_COMPONENTTRANSFORM_H
#define DARKNEC_COMPONENTTRANSFORM_H

#include <State.h>
#include <component/Component.h>
#include <component/ComponentRWUtils.h>

class ComponentTransform : public Component {
public:

	DECL_COMPONENT_FACTORY(ComponentTransform)
	DECL_COMPONENTREQUIREMENT_FACTORY(ComponentTransform)

	ComponentTransform(GameObject* container);

	virtual void init();
	virtual void update() {}
	virtual bool validate();


	virtual void read(XMLNode node);
	virtual XMLNode write(XMLNode node) {
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
