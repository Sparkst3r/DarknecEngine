#include <State.h>


namespace Darknec {
	int GLVersion_MAJOR;
	int GLVersion_MINOR;
	int GLSLVersion;

	RunState RUNSTATE = STOPPED;
	LoggingManager logger = LoggingManager("DarknecLog.txt");
	SDL_Window* window;
	SDL_GLContext glContext;

	bool SDLOK = false;
	bool SDLIMGOK = false;

	ComponentRegistry* componentFactory = new ComponentRegistry();


}