#ifndef DARKNEC_STATE_H
#define DARKNEC_STATE_H
#include <DarknecEngine.h>

/**
* Darknec
* @brief Base namespace
*
* Contains everything
* @author Sparkst3r
* @date 03 July 2014
*/
namespace Darknec {
	const int MAJOR = 1;
	const int MINOR = 0;
	const int PATCH = 4;
	extern int GLVersion_MAJOR;
	extern int GLVersion_MINOR;
	extern int GLSLVersion;

	enum RunState {
		PLAYING,
		SIMULATING,
		PAUSED,
		MENU,
		STOPPED,
		CRASHED,
	};

	extern SDL_Window* window;
	extern SDL_GLContext glContext;
	extern LoggingManager logger;
	extern RunState RUNSTATE;

	extern bool SDLOK;
	extern bool SDLIMGOK;

}

#endif