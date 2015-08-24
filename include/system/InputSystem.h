#ifndef DARKNEC_INPUTSYSTEM_H 
#define DARKNEC_INPUTSYSTEM_H
#include <Core.h>
#include <component/component.h>



class InputSystem {
public:
	InputSystem();

	void keyPressed(GLFWwindow* window, int key, int scancode, int action, int modifier);
	
	void mouseMoved(GLFWwindow* window, double x, double y);

	void registerInputReceiver(Component* comp);

	bool getIsPressed(int key);
	bool getIsPressed(std::string key);

	void pushUpdate();
	std::hash_map<std::string, int> stringToKeyMap;
	std::vector<std::string> keyToStringMap;

	std::vector<bool> keysPressed;
	int lastPressMode = GLFW_RELEASE;
	int lastKey = GLFW_KEY_UNKNOWN;
	std::string lastKeyStr = "UNKNOWN";
	bool keyIsPressed= false;

	std::vector<Component*> inputReceivers;

	double mouse_X_;
	double mouse_Y_;

};

extern InputSystem* sys4;

void glfwInputKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int modifier);

void glfwInputMouseMoveCallback(GLFWwindow* window, double x, double y);

#endif // !DARKNEC_INPUTSYSTEM_H
