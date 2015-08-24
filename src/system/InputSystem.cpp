#include <system/InputSystem.h>



InputSystem::InputSystem() {
	keysPressed.resize(GLFW_KEY_LAST);
	std::fill(keysPressed.begin(), keysPressed.end(), false);

	this->stringToKeyMap[std::string("SPACE")] = GLFW_KEY_SPACE;
	this->stringToKeyMap[std::string("APOSTROPHE")] = GLFW_KEY_APOSTROPHE;
	this->stringToKeyMap[std::string("COMMA")] = GLFW_KEY_COMMA;
	this->stringToKeyMap[std::string("MINUS")] = GLFW_KEY_MINUS;
	this->stringToKeyMap[std::string("PERIOD")] = GLFW_KEY_PERIOD;
	this->stringToKeyMap[std::string("0")] = GLFW_KEY_0;
	this->stringToKeyMap[std::string("1")] = GLFW_KEY_1;
	this->stringToKeyMap[std::string("2")] = GLFW_KEY_2;
	this->stringToKeyMap[std::string("3")] = GLFW_KEY_3;
	this->stringToKeyMap[std::string("4")] = GLFW_KEY_4;
	this->stringToKeyMap[std::string("5")] = GLFW_KEY_5;
	this->stringToKeyMap[std::string("6")] = GLFW_KEY_6;
	this->stringToKeyMap[std::string("7")] = GLFW_KEY_7;
	//#define GLFW_KEY_8                  56
	//#define GLFW_KEY_9                  57
	//#define GLFW_KEY_SEMICOLON          59  /* ; */
	//#define GLFW_KEY_EQUAL              61  /* = */
	this->stringToKeyMap[std::string("A")] = GLFW_KEY_A;
	//#define GLFW_KEY_B                  66
	//#define GLFW_KEY_C                  67
	this->stringToKeyMap[std::string("D")] = GLFW_KEY_D;
	//#define GLFW_KEY_E                  69
	//#define GLFW_KEY_F                  70
	this->stringToKeyMap[std::string("G")] = GLFW_KEY_G;
	//#define GLFW_KEY_H                  72
	//#define GLFW_KEY_I                  73
	//#define GLFW_KEY_J                  74
	//#define GLFW_KEY_K                  75
	//#define GLFW_KEY_L                  76
	//#define GLFW_KEY_M                  77
	//#define GLFW_KEY_N                  78
	//#define GLFW_KEY_O                  79
	//#define GLFW_KEY_P                  80
	//#define GLFW_KEY_Q                  81
	//#define GLFW_KEY_R                  82
	this->stringToKeyMap[std::string("S")] = GLFW_KEY_S;
	//#define GLFW_KEY_T                  84
	//#define GLFW_KEY_U                  85
	//#define GLFW_KEY_V                  86
	this->stringToKeyMap[std::string("W")] = GLFW_KEY_W;
	//#define GLFW_KEY_X                  88
	//#define GLFW_KEY_Y                  89
	//#define GLFW_KEY_Z                  90
	//#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
	//#define GLFW_KEY_BACKSLASH          92  /* \ */
	//#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
	//#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
	//#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
	//#define GLFW_KEY_WORLD_2            162 /* non-US #2 */

	//	/* Function keys */
	this->stringToKeyMap[std::string("ESC")] = GLFW_KEY_ESCAPE;
	//#define GLFW_KEY_ENTER              257
	//#define GLFW_KEY_TAB                258
	//#define GLFW_KEY_BACKSPACE          259
	//#define GLFW_KEY_INSERT             260
	//#define GLFW_KEY_DELETE             261
	//#define GLFW_KEY_RIGHT              262
	//#define GLFW_KEY_LEFT               263
	//#define GLFW_KEY_DOWN               264
	//#define GLFW_KEY_UP                 265
	//#define GLFW_KEY_PAGE_UP            266
	//#define GLFW_KEY_PAGE_DOWN          267
	//#define GLFW_KEY_HOME               268
	//#define GLFW_KEY_END                269
	//#define GLFW_KEY_CAPS_LOCK          280
	//#define GLFW_KEY_SCROLL_LOCK        281
	//#define GLFW_KEY_NUM_LOCK           282
	//#define GLFW_KEY_PRINT_SCREEN       283
	//#define GLFW_KEY_PAUSE              284
	//#define GLFW_KEY_F1                 290
	//#define GLFW_KEY_F2                 291
	//#define GLFW_KEY_F3                 292
	//#define GLFW_KEY_F4                 293
	//#define GLFW_KEY_F5                 294
	//#define GLFW_KEY_F6                 295
	//#define GLFW_KEY_F7                 296
	//#define GLFW_KEY_F8                 297
	//#define GLFW_KEY_F9                 298
	//#define GLFW_KEY_F10                299
	//#define GLFW_KEY_F11                300
	//#define GLFW_KEY_F12                301
	//#define GLFW_KEY_F13                302
	//#define GLFW_KEY_F14                303
	//#define GLFW_KEY_F15                304
	//#define GLFW_KEY_F16                305
	//#define GLFW_KEY_F17                306
	//#define GLFW_KEY_F18                307
	//#define GLFW_KEY_F19                308
	//#define GLFW_KEY_F20                309
	//#define GLFW_KEY_F21                310
	//#define GLFW_KEY_F22                311
	//#define GLFW_KEY_F23                312
	//#define GLFW_KEY_F24                313
	//#define GLFW_KEY_F25                314
	//#define GLFW_KEY_KP_0               320
	//#define GLFW_KEY_KP_1               321
	//#define GLFW_KEY_KP_2               322
	//#define GLFW_KEY_KP_3               323
	//#define GLFW_KEY_KP_4               324
	//#define GLFW_KEY_KP_5               325
	//#define GLFW_KEY_KP_6               326
	//#define GLFW_KEY_KP_7               327
	//#define GLFW_KEY_KP_8               328
	//#define GLFW_KEY_KP_9               329
	//#define GLFW_KEY_KP_DECIMAL         330
	//#define GLFW_KEY_KP_DIVIDE          331
	//#define GLFW_KEY_KP_MULTIPLY        332
	//#define GLFW_KEY_KP_SUBTRACT        333
	//#define GLFW_KEY_KP_ADD             334
	//#define GLFW_KEY_KP_ENTER           335
	//#define GLFW_KEY_KP_EQUAL           336
	this->stringToKeyMap[std::string("LSHIFT")] = GLFW_KEY_LEFT_SHIFT;
	//#define GLFW_KEY_LEFT_CONTROL       341
	//#define GLFW_KEY_LEFT_ALT           342
	//#define GLFW_KEY_LEFT_SUPER         343
	//#define GLFW_KEY_RIGHT_SHIFT        344
	//#define GLFW_KEY_RIGHT_CONTROL      345
	//#define GLFW_KEY_RIGHT_ALT          346
	//#define GLFW_KEY_RIGHT_SUPER        347
	//#define GLFW_KEY_MENU               348
	//#define GLFW_KEY_LAST               GLFW_KEY_MENU
	keyToStringMap.resize(GLFW_KEY_LAST);
	typedef std::hash_map<std::string, int>::iterator it_type;
	for (it_type iterator = stringToKeyMap.begin(); iterator != stringToKeyMap.end(); iterator++) {
		keyToStringMap[iterator->second] = iterator->first;
	}
}

bool InputSystem::getIsPressed(int key) {
	return this->keysPressed[key];
}

bool InputSystem::getIsPressed(std::string key) {
	return this->keysPressed[this->stringToKeyMap[key]];
}

void InputSystem::registerInputReceiver(Component* comp) {
	this->inputReceivers.push_back(comp);
}

void InputSystem::keyPressed(GLFWwindow* window, int key, int scancode, int action, int modifier) {
	this->lastKeyStr = this->keyToStringMap[key];
	this->lastPressMode = action;
	this->keysPressed[key] = true;
	if (action == GLFW_PRESS) {
		this->keysPressed[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		this->keysPressed[key] = false;
	}

	if (std::find(this->keysPressed.begin(), this->keysPressed.end(), true) != this->keysPressed.end()) {
		this->keyIsPressed = true;
	}

	if (std::find(this->keysPressed.begin(), this->keysPressed.end(), true) == this->keysPressed.end()) {
		this->keyIsPressed = true;
	}

}

void InputSystem::mouseMoved(GLFWwindow* window, double x, double y) {
	this->mouse_X_ = x;
	this->mouse_Y_ = y;
}

void InputSystem::pushUpdate() {
	for (Component* rec : inputReceivers) {
		rec->update();
	}
}


InputSystem* sys4 = new InputSystem();

void glfwInputKeyPressCallback(GLFWwindow* window, int key, int scancode, int action, int modifier) {
	sys4->keyPressed(window, key, scancode, action, modifier);
}

void glfwInputMouseMoveCallback(GLFWwindow* window, double x, double y) {
	sys4->mouseMoved(window, x, y);
}