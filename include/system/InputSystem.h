#ifndef DARKNEC_INPUTSYSTEM_H 
#define DARKNEC_INPUTSYSTEM_H
#include <Core.h>
#include <component/ComponentInput.h>



class InputReceiver {
public:

	InputReceiver(ComponentInput* comp, InputType type) {
		_comp = comp;
		_type = type;
	}
	ComponentInput* _comp;
	InputType _type;
};

class InputSystem {
public:
	InputSystem();

	void keyPressed(GLFWwindow* window, int key, int scancode, int action, int modifier);
	
	void mouseMoved(GLFWwindow* window, double x, double y);

	void registerInputReceiver(ComponentInput* comp, InputType type);

	void pushUpdate();
	std::hash_map<std::string, int> stringToKeyMap;

	std::vector<bool> keysPressed;
	int lastPressMode = GLFW_RELEASE;
	bool canRetrigger = true;
	int lastKey = GLFW_KEY_UNKNOWN;

	std::vector<InputReceiver> inputReceivers;

	double mouse_X_;
	double mouse_Y_;

};

extern InputSystem* sys4;

void glfwInputKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int modifier);

void glfwInputMouseMoveCallback(GLFWwindow* window, double x, double y);

#endif // !DARKNEC_INPUTSYSTEM_H
