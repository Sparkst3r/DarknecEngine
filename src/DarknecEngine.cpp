#include <DarknecEngine.h>

#include <stdio.h>
#include <stdlib.h>
#include <SDL_Image.h>
#include <sstream>

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

	Logger darknecLogger;

	///Internal function
	//Setup vital engine libraries and state.
	//	$settings	: Settings structure
	int InitEngine(Darknec::Callback::Settings* settings) {

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

		//TODO Remove this when texture loading is fixed
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

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
				glGetIntegerv(GL_MAJOR_VERSION, &GLVersion_MAJOR);
				glGetIntegerv(GL_MINOR_VERSION, &GLVersion_MINOR);

				std::string contextString;
				int contextAttribute;
				SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &contextAttribute);
				if (contextAttribute == SDL_GL_CONTEXT_PROFILE_COMPATIBILITY) {
					contextString = "compatibility";
				}
				else {
					contextString = "core";
				}



				int glslVersion = GLVersion_MAJOR * 100 + GLVersion_MINOR * 10;


				if (glslVersion >= 330) {
				}
				else if (glslVersion >= 320) {
					glslVersion = 150;
				}
				else if (glslVersion >= 310) {
					glslVersion = 140;
				}
				else if (glslVersion >= 300) {
					glslVersion = 130;
				}
				else if (glslVersion >= 210) {
					glslVersion = 120;
				}
				else if (glslVersion >= 200) {
					glslVersion = 110;
				}
				GLSLVersion = glslVersion;


				darknecLogger(LogLevel::LOG_LOG, "GL Context set up successfully");
				darknecLogger(LogLevel::LOG_INFO, "GPU vendor: %s", glGetString(GL_VENDOR));
				darknecLogger(LogLevel::LOG_INFO, "Using OpenGL specification version: %s %s ", glGetString(GL_VERSION), contextString.c_str());
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



}

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
	* DarknecInit
	* @brief Starts DarknecEngine
	*
	* Setup DarknecEngine with settings.
	* This must be called after callback functions are defined.
	*
	* @param settings settings in which to set-up the engine
	* @param argc argument count
	* @param argv argument values
	*/
	void DarknecInit(Darknec::Callback::Settings* settings, int argc, char* argv[]) {

		darknecLogger = DLogger.getLogger("DarknecEngine");

		darknecLogger(LogLevel::LOG_SECTION, "============-START-=============");
		darknecLogger(LogLevel::LOG_LOG, "Darknec starting up");

		int success = InitEngine(settings);
		if (success != 0) {
			darknecLogger(LogLevel::LOG_FATAL, "FATAL ERROR. Darknec could not initialise. Exit code: %i", success);
			DarknecShutdown(success);
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
			DarknecShutdown(5);
		}

		GameLoopBegin();
	}

	/**
	* DarknecShutdown
	* @brief Shuts down DarknecEngine
	*
	* Shutdown DarknecEngine with a close code.
	*
	* @param close Code to quit with
	*/
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
