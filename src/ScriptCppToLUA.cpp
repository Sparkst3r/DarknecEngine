#include <ScriptCpptoLUA.h>
#include <system/GameObjectSystem.h>
#include <system/ShaderSystem.h>
#include <system/InputSystem.h>

#include <component/ComponentTransform.h>
#include <component/ComponentCamera.h>
#include <component/ComponentMesh.h>
//#include <component/ComponentScript.h>
#include <component/ComponentData.h>
#include <State.h>

class LuaEngineInterface {
public:
	int GLFW_CURSOR_ = GLFW_CURSOR;
	int GLFW_CURSOR_HIDDEN_ = GLFW_CURSOR_HIDDEN;
	int GLFW_CURSOR_NORMAL_ = GLFW_CURSOR_NORMAL;

	int MOUSE_RELEASE_ = GLFW_RELEASE;

	int PLAYING = 0;
	int SIMULATING = 1;
	int PAUSED = 2;
	int MENU = 3;
	int STOPPED = 4;
	int CRASHED = 5;

	inline GLFWwindow* getWindow() {
		return Darknec::Detail::window;
	}

	inline void setRunstate(int runstate) {
		Darknec::RUNSTATE = Darknec::RunState(runstate);
	}

	inline int getRunstate() {
		return Darknec::RUNSTATE;
	}

	inline glm::vec2 getWindowDimensions() {
		return glm::vec2(Darknec::WindowWidth, Darknec::WindowHeight);
	}
};

LuaEngineInterface luaInterfaceI = LuaEngineInterface();

void setCursorPos(int x, int y) {
	glfwSetCursorPos(Darknec::Detail::window, x, y);
}

void setCursorMode(int mode) {
	glfwSetInputMode(Darknec::Detail::window, GLFW_CURSOR, mode);
}

namespace Darknec {
	void registerDarknec() {
		luabind::class_<LuaEngineInterface> luaInterface = luabind::class_<LuaEngineInterface>("Darknec");
		luaInterface.def("getWindowDimensions", &LuaEngineInterface::getWindowDimensions);
		luaInterface.def("getRunstate", &LuaEngineInterface::getRunstate);
		luaInterface.def("setRunstate", &LuaEngineInterface::setRunstate);

		luaInterface.def_readonly("CURSOR",			&LuaEngineInterface::GLFW_CURSOR_);
		luaInterface.def_readonly("CURSOR_HIDDEN",	&LuaEngineInterface::GLFW_CURSOR_HIDDEN_);
		luaInterface.def_readonly("CURSOR_NORMAL",	&LuaEngineInterface::GLFW_CURSOR_NORMAL_);

		luaInterface.def_readonly("MOUSE_RELEASE", &LuaEngineInterface::MOUSE_RELEASE_);
		
		luaInterface.def_readonly("STOPPED", &LuaEngineInterface::STOPPED);


		luabind::module(LUA)[luabind::def("setCursorPos", &setCursorPos)];
		luabind::module(LUA)[luabind::def("setCursorMode", &setCursorMode)];


		luabind::module(LUA)[luaInterface];
		luabind::globals(LUA)["Darknec"] = luaInterfaceI;
	}

	void registerComponents() {
		luabind::class_<GameObject> gameObject = luabind::class_<GameObject>("GameObject");
		gameObject.def("getComponent", &GameObject::getComponent);
		gameObject.def("hasComponent", &GameObject::hasComponent);
		gameObject.def("getName", &GameObject::getName);
		
		luabind::class_<Component> component = luabind::class_<Component>("Component");
		luabind::class_<ComponentTransform, Component> componentTransform = luabind::class_<ComponentTransform, Component>("ComponentTransform");
		luabind::class_<ComponentCamera, Component> componentCamera = luabind::class_<ComponentCamera, Component>("ComponentCamera");
		luabind::class_<ComponentData, Component> componentData = luabind::class_<ComponentData, Component>("ComponentData");

		component.def_readwrite("typeName_", &Component::typeName_);
		component.def("getName", &Component::getName);

		componentTransform.def("setPosition", &ComponentTransform::setPosition);
		componentTransform.def("getPosition", &ComponentTransform::getPosition);
		componentTransform.def("offsetPosition", &ComponentTransform::offsetPosition);
		componentTransform.def("setOrientation", &ComponentTransform::setOrientation);
		componentTransform.def("getOrientation", &ComponentTransform::getOrientation);

		componentCamera.def("generateViewMatrix", &ComponentCamera::generateViewMatrix);

		componentData.def("setBool", &ComponentData::setBool);
		componentData.def("getBool", &ComponentData::getBool);
		componentData.def("setFloat", &ComponentData::setFloat);
		componentData.def("getFloat", &ComponentData::getFloat);

		luabind::module(LUA)[gameObject];
		luabind::module(LUA)[component];
		luabind::module(LUA)[componentTransform];
		luabind::module(LUA)[componentCamera];
		luabind::module(LUA)[componentData];
	}
	glm::fquat quatCast(glm::mat4 mat) {
		return glm::quat_cast(mat);
	}

	glm::fquat conjugate(glm::fquat quat) {
		return glm::conjugate(quat);
	}
	glm::fquat angleAxis(float f, glm::vec3 vec) {
		return glm::angleAxis(f, vec);
	}

	void registerMaths() {
		//Maths
		luabind::class_<glm::vec2> Vec2 = luabind::class_<glm::vec2>("vec2");
		luabind::class_<glm::vec3> Vec3 = luabind::class_<glm::vec3>("vec3");
		luabind::class_<glm::vec4> Vec4 = luabind::class_<glm::vec4>("vec4");

		Vec2.def(luabind::constructor<>());
		Vec2.def(luabind::constructor<int, int>());
		Vec2.def_readwrite("x", &glm::vec2::x);
		Vec2.def_readwrite("y", &glm::vec2::y);

		Vec3.def(luabind::constructor<>());
		Vec3.def(luabind::constructor<int, int, int>());
		Vec3.def_readwrite("x", &glm::vec3::x);
		Vec3.def_readwrite("y", &glm::vec3::y);
		Vec3.def_readwrite("z", &glm::vec3::z);
		Vec3.def(luabind::self * glm::core::type::precision::mediump_float());
		Vec3.def(luabind::self * glm::detail::tvec3<glm::core::type::precision::mediump_float>());

		luabind::module(LUA)[Vec2];
		luabind::module(LUA)[Vec3];

		luabind::class_<glm::mat4> Mat4 = luabind::class_<glm::mat4>("mat4");
		luabind::module(LUA)[Mat4];

		luabind::class_<glm::fquat> fQuat = luabind::class_<glm::fquat>("fquat");
		fQuat.def(luabind::self * glm::detail::tvec3<glm::core::type::precision::mediump_float>());
		fQuat.def(luabind::self * glm::detail::tquat<glm::core::type::precision::mediump_float>());
		luabind::module(LUA)[fQuat];

		luabind::module(LUA)[luabind::def("conjugate", &conjugate)];
		luabind::module(LUA)[luabind::def("quat_cast", &quatCast)];
		luabind::module(LUA)[luabind::def("angleAxis", &angleAxis)];

		luabind::module(LUA)[luabind::def("sin", static_cast<float (*)(float)>(&std::sin))];
		luabind::module(LUA)[luabind::def("floor", static_cast<float(*)(float)>(&std::floor))];
		luabind::module(LUA)[luabind::def("ceil", static_cast<float(*)(float)>(&std::ceil))];
	}



	void doRegisterCppLUAFuncs() {
		try {
			registerDarknec();
			registerComponents();

			registerMaths();
			

			//Systems
			luabind::class_<GameObjectSystem> gameObjectSystem = luabind::class_<GameObjectSystem>("GameObjectSystem");
			//luabind::class_<ShaderSystem> shaderSystem = luabind::class_<ShaderSystem>("ShaderSystem");
			luabind::class_<InputSystem> inputSystem = luabind::class_<InputSystem>("InputSystem");

			gameObjectSystem.def("registerObject", &GameObjectSystem::registerObject);

			inputSystem.def("getIsPressed", static_cast<bool (InputSystem::*)(int)>(&InputSystem::getIsPressed));
			inputSystem.def("getIsPressed", static_cast<bool (InputSystem::*)(std::string)>(&InputSystem::getIsPressed));
			inputSystem.def_readonly("isKeyPressed", &InputSystem::keyIsPressed);
			inputSystem.def_readonly("lastPressMode", &InputSystem::lastPressMode);
			inputSystem.def_readonly("lastKey", &InputSystem::lastKeyStr);
			inputSystem.def_readonly("mouse_X", &InputSystem::mouse_X_);
			inputSystem.def_readonly("mouse_Y", &InputSystem::mouse_Y_);
			
			luabind::module(LUA)[gameObjectSystem];
			luabind::module(LUA)[inputSystem];

			luabind::globals(LUA)["GameObjectSystem"] = sys2;
			luabind::globals(LUA)["InputSystem"] = sys4;
		}
		catch (const std::exception &TheError) {
			std::cerr << TheError.what() << std::endl;
			Darknec::logger("ad");
		}


	}
}