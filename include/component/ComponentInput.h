#ifndef DARKNEC_COMPONENTINPUT_H
#define DARKNEC_COMPONENTINPUT_H

#include <State.h>
#include <component/Component.h>
#include <component/ComponentTransform.h>
#include <component/ComponentRWUtils.h>


class ComponentInput : public Component {
public:

	DECL_COMPONENT_FACTORY(ComponentInput)
	DECL_COMPONENTREQUIREMENT_FACTORY(ComponentInput)

		ComponentInput(GameObject* container);

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

	InputType type = InputType::KEYBOARD;


private:
	void OffsetCameraDebug(glm::vec3 offset);
};
#endif // !DARKNEC_COMPONENTINPUT_H
