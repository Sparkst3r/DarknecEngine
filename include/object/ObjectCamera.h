#ifndef DARKNEC_OBJECTCAMERA_H
#define DARKNEC_OBJECTCAMERA_H
#include <object/Object.h>


class ObjectCamera : public Object {

public:

	ObjectCamera(glm::vec3 position, glm::fquat rotation);

	ObjectCamera(glm::vec3 position, glm::fquat rotation, float distance);

	glm::mat4 generateViewMatrix();
	glm::mat4 generateProjectionMatrix();

	void ObjectCamera::offsetDistance(float offset);

	float getDistance() const;
	int   getWidth() const;
	int   getHeight() const;

	void  setDistance(float distance);
	void  setProjectionBounds(int width, int height);
	void  setProjectionClipBounds(float nearClip, float farClip);
	void  setFieldOfView(float FOV);


private:
	int	  width;
	int   height;
	float fov;
	float nearClippingPlane;
	float farClippingPlane;
	float distance;
};

#endif