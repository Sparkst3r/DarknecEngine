#include <State.h>


namespace Darknec {
	RunState RUNSTATE = STOPPED;
	LoggingManager DLogger("DarknecLog.txt");
	SDL_Window* window;
	SDL_GLContext glContext;
}