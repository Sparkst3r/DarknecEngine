#ifndef DARKNEC_COMPONENTCAMERA_H
#define DARKNEC_COMPONENTCAMERA_H

#include <State.h>
#include <component/Component.h>
#include <component/ComponentTransform.h>
#include <component/ComponentRWUtils.h>

class ComponentCamera : public Component {
public:

	enum ProjType {
		PERSPECTIVE,
		ORTHOGRAPHIC,
	};

	DECL_COMPONENT_FACTORY(ComponentCamera)
	DECL_COMPONENTREQUIREMENT_FACTORY(ComponentCamera)

	ComponentCamera(GameObject* container);

	virtual void update() {}
	virtual void read(rapidxml::xml_node<>* node);

	virtual rapidxml::xml_node<>* write(rapidxml::xml_node<>*) {
		return NULL;
	}

	glm::mat4 generateViewMatrix();
	glm::mat4 generateProjMatrix();

	float getDistance() const;
	void setDistance(float distance);
	void offsetDistance(float offsetDistance);

	bool getProjType() const;
	void setProjType(ComponentCamera::ProjType projType);
	
	float getFOV() const;
	void setFOV(float FOV);

	void setCaptureDimension(int width, int height);
	void setClipPlanes(float nearClip, float farClip);


private:

	ComponentRequirement<ComponentTransform> transform_;

	ComponentCamera::ProjType projectionType_;
	float FOV_;

	int width_;
	int height_;
	float frustrumNearClip_;
	float frustrumFarClip_;

	float distance_;


};
#endif // !DARKNEC_COMPONENT_H
