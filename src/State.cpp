#include <State.h>


namespace Darknec {
	int GLVersion_MAJOR;
	int GLVersion_MINOR;
	int GLSLVersion;

	RunState RUNSTATE = STOPPED;
	LoggingManager logger = LoggingManager("DarknecLog.txt");
	SDL_Window* window;
	SDL_GLContext glContext;

	extern bool SDLOK = false;
	extern bool SDLIMGOK = false;

}