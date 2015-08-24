#include <component/ComponentScript.h>
#include <system/InputSystem.h>
#include <component/ComponentCamera.h>
#include <component/ComponentData.h>
/**
* ComponentCamera
*
* Constructor
*/
ComponentScript::ComponentScript(GameObject* container) {
	this->container_ = container;
	this->typeName_ = "ComponentScript";
}

bool ComponentScript::validate() {
	return true;
}

void ComponentScript::init() {
	sys4->registerInputReceiver(this);
}

bool grabbing = false;
bool cangrab = true;

void ComponentScript::update() {
	try {
		int err = luaL_dofile(Darknec::LUA, this->script_.c_str());
		if (err) {
			Darknec::logger("Could not compile script. Error: %s", lua_tostring(Darknec::LUA, -1));
		}
		else {
			luabind::call_function<void>(Darknec::LUA, "script", this->container_);
		}

	}
	catch (const std::exception &TheError) {
		Darknec::logger("error: lua: %s", lua_tostring(Darknec::LUA, -1));
		Darknec::logger(TheError.what());
	}
	glm::vec3 direction = glm::vec3();
	/*if (sys4->keysPressed[GLFW_KEY_ESCAPE]) {
		Darknec::RUNSTATE = Darknec::STOPPED;
	}*/

}
/**
* read
*
* Load component data from xml.
* @param node root node of the component to read from.
*/
void ComponentScript::read(XMLNode node) {
	this->script_ = Darknec::baseAssetPath + Darknec::ComponentRWUtils::readString(node, "script");
}