#include <DarknecEngine.h>

#include <stdio.h>
#include <stdlib.h>
#include <SDL_Image.h>
#include <sstream>

#include <LoggingManager.h>


//Core Namespace
namespace Darknec {
	using namespace Darknec::Callback;

	Logger darknecLogger;

	///Internal function
	//Setup vital engine libraries and state.
	//	$settings	: Settings structure
	int InitEngine(Settings* settings) {

		///Setup SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			darknecLogger(LogLevel::LOG_FATAL, "Could not initialise SDL");
			return DarknecShutdown(1);
		}
		else {
			darknecLogger(LogLevel::LOG_LOG, "SDL Initialised");
		}

		///Setup SDL_Image
		if (IMG_Init(IMG_INIT_PNG) == 0) {
			darknecLogger(LogLevel::LOG_FATAL, "Could not initialise SDL_Image");
			return DarknecShutdown(2);
		}
		else {
			darknecLogger(LogLevel::LOG_LOG, "SDL_Image extension initialised");
		}

		///Set GL attributes
		for (Darknec::Callback::WindowAttribute attr : settings->attributes) {
			SDL_GL_SetAttribute(attr.attr, attr.value);
		}
		darknecLogger(LogLevel::LOG_LOG, "GL window attributes loaded");

		///Create window
		window = SDL_CreateWindow(settings->windowName.c_str(), settings->windowX, settings->windowY, settings->windowWidth, settings->windowHeight, settings->windowFlags);
		if (!window) {
			darknecLogger(LogLevel::LOG_FATAL, "Could not create window");
			return DarknecShutdown(3);
		}

		///Attempt to set default window icon
		if (settings->windowIcon != NULL) {
			SDL_Surface* icon = IMG_Load("DarknecIcon.png");
			if (icon) {
				SDL_SetWindowIcon(window, icon);
			}
			else {
				darknecLogger(LogLevel::LOG_WARN, "Could not load default icon. Reverting to OS.");
			}
		}
		
		darknecLogger(LogLevel::LOG_INFO, "Window created with flag : %i", settings->windowFlags);
		
		delete settings;

		///Create GL context
		glContext = SDL_GL_CreateContext(window);
		

		if (glContext == NULL) {
			darknecLogger(LogLevel::LOG_FATAL, "Unable to create a GL context");
			return DarknecShutdown(4);
		}
		else {
			glewExperimental = GL_TRUE;
			int glewError = glewInit();
			if (glewError != GLEW_OK) {
				darknecLogger(LogLevel::LOG_FATAL, "Could not initialise GLEW. Error: %s", glewGetErrorString(glewError));
			}
			else {
				darknecLogger(LogLevel::LOG_LOG, "GL Context set up successfully");
				darknecLogger(LogLevel::LOG_INFO, "GPU vendor: %s", glGetString(GL_VENDOR));
				darknecLogger(LogLevel::LOG_INFO, "Using OpenGL specification version: %s", glGetString(GL_VERSION));
				darknecLogger(LogLevel::LOG_INFO, "Using GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
			}
		}

		RUNSTATE = PLAYING;
		return 0;
	}

	///Internal function
	//Enter game loop
	void GameLoopBegin() {
		SDL_Event currentEvent;

		while (Darknec::RUNSTATE != Darknec::STOPPED && Darknec::RUNSTATE != Darknec::CRASHED) {
			while (SDL_PollEvent(&currentEvent)) {

				if (currentEvent.type == SDL_QUIT) {
					Darknec::RUNSTATE = Darknec::STOPPED;

				}

				if (Darknec::Callback::getEventCallback() != NULL) {
					Darknec::Callback::getEventCallback()(currentEvent);
				}




				if (currentEvent.type == SDL_WINDOWEVENT && currentEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
					if (Darknec::Callback::getResizeCallback() != NULL) {
						Darknec::Callback::getResizeCallback()(currentEvent);
					}
				}
			}

			if (Darknec::Callback::getLogicCallback() != NULL) {
				Darknec::Callback::getLogicCallback()();
			}


#pragma region Render
			if (Darknec::Callback::getRenderCallback() != NULL) {
				Darknec::Callback::getRenderCallback()(window);
			}
			else {
				darknecLogger(LogLevel::LOG_FATAL, "Illegal State: NULL render method is not supported. Define a render method");
				DarknecShutdown(6);
			}
#pragma endregion
		}
	}


	///Exposed function
	//Start engine
	//	$settings	: Settings structure
	//	$argc		: Argument count
	//	$argv		: Argument values
	void DarknecInit(Settings* settings, int argc, char* argv[]) {

		darknecLogger = DLogger.getLogger("DarknecEngine");

		darknecLogger(LogLevel::LOG_SECTION, "============-START-=============");
		darknecLogger(LogLevel::LOG_LOG, "Darknec starting up");
		
		int success = InitEngine(settings);
		if (success != 0) {
			darknecLogger(LogLevel::LOG_FATAL, "FATAL ERROR. Darknec could not initialise. Exit code: %i", success);
			exit(success);

		}
		
		darknecLogger(LogLevel::LOG_LOG, "Darknec started successfully");

		darknecLogger(LogLevel::LOG_SECTION, "======-END DARKNEC SETUP-=======");
		darknecLogger(LogLevel::LOG_SECTION, "==========-START GAME-==========");
		if (Darknec::Callback::getInitCallback() != NULL) {
			darknecLogger(LogLevel::LOG_LOG, "Running userdef init");
			Darknec::Callback::getInitCallback()(argc, argv, window, glContext);
		}
		else {
			darknecLogger(LogLevel::LOG_FATAL, "Illegal State: NULL init method is not supported. Define an init method");
			exit(5);
		}

		GameLoopBegin();
	}

	///Exposed function
	//Shutdown engine.
	//	$close		: Close code to shutdown with
	int DarknecShutdown(int close) {
		if (close != 1 && close != 2 && close != 3 && close != 4) {
			if (Darknec::Callback::getCleanupCallback() != NULL) {
				Darknec::Callback::getCleanupCallback()();
			}
			SDL_GL_DeleteContext(glContext);
		}
		if (close != 1 && close != 2 && close != 3) {
			SDL_DestroyWindow(window);
		}
		if (close != 1 && close != 2) {
			IMG_Quit();
		}
		if (close != 1) {
			SDL_Quit();
		}
		darknecLogger(LogLevel::LOG_SECTION, "=============-END-==============");
		return close;
	}
}
