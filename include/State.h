#ifndef DARKNEC_STATE_H
#define DARKNEC_STATE_H

#include <Core.h>

#include <Darknec_Callbacks.h>
#include <LoggingManager.h>
/**
* Darknec
* @brief Base namespace
*
* Contains everything
* @author Sparkst3r
* @date 03 July 2014
*/
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
		extern SDL_Window* window;
		/**
		* Internal context handle.
		*/
		extern SDL_GLContext glContext;

		/**
		* Set to true when SDL has initialised correctly.
		*/
		extern bool SDLOK;
		/**
		* Set to true when SDLIMG has initialised correctly.
		*/
		extern bool SDLIMGOK;
	}

	/**
	* Darknec Major version. 
	* Incremented when an update will break games. Eg. Big rewrites and refactors
	*/
	const int MAJOR = 1;
	/**
	* Darknec Minor version.
	* Incremented when an update introduces a new feature. Eg. Terrain generation
	*/
	const int MINOR = 1;
	/**
	* Darknec Patch version.
	* Incremented when I break something and need to hotfix it. Eg. Spelling mistakes or to fix a bug.
	*/
	const int PATCH = 0;

	/**
	* OpenGL Major version.
	*/
	extern int GLVersion_MAJOR;

	/**
	* OpenGL Minor version.
	*/
	extern int GLVersion_MINOR;

	/**
	* GLSL version.
	*/
	extern int GLSLVersion;

	enum RunState {
		PLAYING,
		SIMULATING,
		PAUSED,
		MENU,
		STOPPED,
		CRASHED,
	};

	extern int RenderFPS;

	extern LoggingManager logger;
	extern RunState RUNSTATE;
	extern HSQUIRRELVM squirrel;
	extern Sqrat::Table darknec;
	extern std::string baseAssetPath;
}

#endif