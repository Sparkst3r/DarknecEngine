#ifndef DARKNEC_CORE_H 
#define DARKNEC_CORE_H
///Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <hash_map>
#include <time.h>
#include <chrono>
#include <numeric>





///GLEW implementation
#include <GL/glew.h>

///GLFW libraries
#include <GLFW/GLFW3.h>

///Third party libraries
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <rapidXML/rapidXML.hpp>
#include <luabind/lua_include.hpp>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
extern "C"
{
#include <lualib.h>
}

//#include <vld.h>


///Typedefs
#include <render/GLTypeDef.h>

enum InputType {
	KEYBOARD,
	MOUSE,
	JOYSTICK
};


#endif // !DARKNEC_CORE_H


