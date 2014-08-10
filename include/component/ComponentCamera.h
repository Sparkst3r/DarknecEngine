#ifndef DARKNEC_COMPONENTCAMERA_H
#define DARKNEC_COMPONENTCAMERA_H

#include <component/Component.h>
#include <object/GameObject.h>

#include <glm/glm.hpp>

class ComponentCamera : public Component {
public:
	ComponentCamera(GameObject* container);

	virtual void update() {}
	virtual void read(rapidxml::xml_node<>* node) {}

	virtual rapidxml::xml_node<>* write(rapidxml::xml_node<>*) {
		return NULL;
	}

	glm::mat4 generateViewMatrix();
	glm::mat4 generateProjMatrix();

	void setPositionID(int positionID);

	float getDistance() const;
	void setDistance(float distance);
	void offsetDistance(float offsetDistance);

	bool getProjType() const;
	void setProjType(bool type);
	
	float getFOV() const;
	void setFOV(float FOV);

	void setCaptureDimension(int width, int height);
	void setClipPlanes(float nearClip, float farClip);


private:

	const char* positionID_;

	bool projectionType_; //FALSE = Perspective; TRUE = Orthographic
	float FOV_;

	int width_;
	int height_;
	float frustrumNearClip_;
	float frustrumFarClip_;

	float distance_;


};
#endif // !DARKNEC_COMPONENT_H
