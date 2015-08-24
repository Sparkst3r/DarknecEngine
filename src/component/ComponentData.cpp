#include <component/ComponentData.h>

/**
* ComponentCamera
*
* Constructor
*/
ComponentData::ComponentData(GameObject* container) {
	this->container_ = container;
	this->typeName_ = "ComponentData";
}

void ComponentData::init() {
}

/**
* read
*
* Load component data from xml.
* @param node root node of the component to read from.
*/
void ComponentData::read(XMLNode node) {
	for (rapidxml::xml_node<>* dataIter = node->first_node(); dataIter; dataIter = dataIter->next_sibling()) {
		if (std::string(dataIter->name()) == std::string("data")) {
			if (dataIter->first_attribute()->value() == std::string("bool")) {
				bool val = false;
				if (dataIter->value() == std::string("true")) {
					val = true;
				}
				this->boolMap[dataIter->last_attribute()->value()] = val;
			}
			else if (dataIter->first_attribute()->value() == std::string("float")) {
				double val = atof(dataIter->value());
				Darknec::logger("%f", val);
				this->floatMap[dataIter->last_attribute()->value()] = val;
			}
		}
	}
}

void ComponentData::update() {
}
