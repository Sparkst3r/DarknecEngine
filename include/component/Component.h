#ifndef DARKNEC_COMPONENT_H
#define DARKNEC_COMPONENT_H

#include <State.h>
#include <system/ComponentSystem.h>
#include <object/GameObject.h>

typedef rapidxml::xml_node<>* XMLNode;

/**
* Component
*
* @class Component Component.h "include/component/Component.h"
* @brief Single behaviour in a GameObject
*
* 
* Components provide the functionality of Gameobjects. They can be added and removed to provide a feature to an object.
*
*/
class Component {
public:

	enum Stage {
		UPDATE_PRERENDER	= 1 << 1,
		UPDATE_RENDER		= 1 << 2,
		UPDATE_POSTRENDER	= 1 << 3,
	};

	Stage getValidStages();

	/**
	* init
	*
	* Run after all components are loaded and if validate() returns true
	* ensuring all requirements exist and are init'd.
	*/
	virtual void init() = 0;
	
	/**
	* update
	* 
	* Called every tick. Order of invocation based on tick priority. Eg physics before rendering.
	*
	*/
	virtual void update() = 0;

	/**
	* validate
	*
	* Ensure the component's dependancies are valid and the component can safely initiate.
	*
	* @return if the component dependanices are valid and exist.
	*/
	virtual bool validate() = 0;

	/**
	* read
	* 
	* Load component data from xml.
	* @param node root node of the component to read from.
	*/
	virtual void read(XMLNode node) = 0;

	/**
	* write
	*
	* Write component data to xml.
	* @param node root node of the component to write to.
	*/
	virtual XMLNode write(XMLNode node) = 0;



	virtual std::string getName() const {
		return this->name_;
	}

	virtual void setName(std::string name) {
		this->name_ = name;
	}

	
protected:
	GameObject* container_;
	std::string name_;
};

#endif // !DARKNEC_COMPONENT_H
