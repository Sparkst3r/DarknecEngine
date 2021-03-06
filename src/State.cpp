#include <State.h>


namespace Darknec {
	/**
	* Detail
	* @brief Do not touch anything in here. Here be dragons
	*
	* Contains internal functions.
	* @author Sparkst3r
	* @date 15 July 2014
	*/
	namespace Detail {
		/**
		* Internal window handle.
		*/
		GLFWwindow* window;
		///**
		//* Internal context handle.
		//*/
		//SDL_GLContext glContext;

		/**
		* Set to true when SDL has initialised correctly.
		*/
		bool SDLOK;
		/**
		* Set to true when SDLIMG has initialised correctly.
		*/
		bool SDLIMGOK;
	}



	int GLVersion_MAJOR;
	int GLVersion_MINOR;
	int GLSLVersion;

	int WindowWidth;
	int WindowHeight;

	int RenderFPS;
	lua_State* LUA = luaL_newstate();

	//LuaEngineInterface luaInterface = LuaEngineInterface();

	RunState RUNSTATE = STOPPED;
	LoggingManager logger = LoggingManager("DarknecLog.txt");

	std::string baseAssetPath;
}