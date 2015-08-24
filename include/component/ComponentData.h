#ifndef DARKNEC_COMPONENTDATA_H
#define DARKNEC_COMPONENTDATA_H

#include <State.h>
#include <component/Component.h>
#include <component/ComponentRWUtils.h>

class ComponentData : public Component {
public:

	DECL_COMPONENT_FACTORY(ComponentData)
	DECL_COMPONENTREQUIREMENT_FACTORY(ComponentData)

	ComponentData(GameObject* container);

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
	virtual bool validate() {
		return true;
	}

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

	inline bool getBool(std::string str) {
		return boolMap[str];
	}
	inline void setBool(std::string str, bool value) {
		boolMap[str] = value;
	}

	inline int getInt(std::string str)	{
		return intMap[str];
	}
	inline void setInt(std::string str, int value) {
		intMap[str] = value;
	}

	inline double getFloat(std::string str) {
		return floatMap[str];
	}
	inline void setFloat(std::string str, double value) {
		floatMap[str] = value;
	}


private:

	std::hash_map<std::string, bool> boolMap;
	std::hash_map<std::string, int> intMap;
	std::hash_map<std::string, double> floatMap;

};
#endif // !DARKNEC_COMPONENTDATA_H