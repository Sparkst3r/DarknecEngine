#ifndef DARKNEC_COMPONENTCAMERA_H
#define DARKNEC_COMPONENTCAMERA_H

#include <State.h>
#include <component/Component.h>
#include <component/ComponentTransform.h>
#include <component/ComponentRWUtils.h>

/**
* ComponentCamera
*
* @class ComponentCamera ComponentCamera.h "include/component/ComponentCamera.h"
* @brief Single behaviour in a GameObject
*
*
* Camera object. Calculates transform matrices.
*
*/
class ComponentCamera : public Component {
public:

	enum ProjType {
		PERSPECTIVE,
		ORTHOGRAPHIC,
	};

	DECL_COMPONENT_FACTORY(ComponentCamera)
	DECL_COMPONENTREQUIREMENT_FACTORY(ComponentCamera)

	ComponentCamera(GameObject* container);

	/**
	* init
	*
	* Run after all components are loaded and if validate() returns true
	* ensuring all requirements exist and are init'd.
	*/
	virtual void init();

	/**
	* update
	*
	* Called every tick. Order of invocation based on tick priority. Eg physics before rendering.
	*
	*/
	virtual void update();

	/**
	* validate
	*
	* Ensure the component's dependancies are valid and the component can safely initiate.
	*
	* @return if the component dependanices are valid and exist.
	*/
	virtual bool validate();

	/**
	* read
	*
	* Load component data from xml.
	* @param node root node of the component to read from.
	*/
	virtual void read(XMLNode node);

	/**
	* write
	*
	* Write component data to xml.
	* @param node root node of the component to write to.
	*/
	virtual XMLNode write(XMLNode node) {
		return NULL;
	}

	glm::mat4 generateViewMatrix();
	glm::mat4 generateProjMatrix();

	void resizeViewport(int width, int height);
	void bindMatrixBuffers();

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

	UBO globalMatrices_;

	float FOV_;

	int width_;
	int height_;
	float frustrumNearClip_;
	float frustrumFarClip_;

	float distance_;


};
#endif // !DARKNEC_COMPONENTCAMERA_H
