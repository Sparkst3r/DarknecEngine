#ifndef DARKNEC_STATE_H
#define DARKNEC_STATE_H

#include <LoggingManager.h>
#include <SDL.h>

//Darknec namespace
//Exposes useful state and engine details
namespace Darknec {
	const int MAJOR = 1;
	const int MINOR = 0;
	const int PATCH = 2;

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
	extern LoggingManager DLogger;
	extern RunState RUNSTATE;

}

#endif