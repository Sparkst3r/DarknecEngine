/**
* @file DarknecEngine.cpp
* Engine core
*/

#include <DarknecEngine.h>

/**
* @namespace Darknec
* @brief Base namespace
*
* Contains everything
* @author Sparkst3r
* @date 03 July 2014
*/
namespace Darknec {
	/**
	* detail
	* @brief Do not touch anything in here. Here be dragons
	* 
	* Contains internal functions. 
	* @author Sparkst3r
	* @date 15 July 2014
	*/
	namespace detail {

		/**
		* InitEngine
		* Setup engine libs and state
		*
		* @param settings settings for engine
		* @return error state. Set to 0 if setup is successful
		*/
		int InitEngine(Darknec::Callback::Settings* settings) {

			///Setup SDL
			if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
				Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Could not initialise SDL");
				return DarknecShutdown(1);
			}
			else {
				Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "SDL Initialised");
			}

			///Setup SDL_Image
			if (IMG_Init(IMG_INIT_PNG) == 0) {
				Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Could not initialise SDL_Image");
				return DarknecShutdown(2);
			}
			else {
				Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "SDL_Image extension initialised");
			}

			///Set GL attributes
			for (Darknec::Callback::WindowAttribute attr : settings->attributes) {
				SDL_GL_SetAttribute(attr.attr, attr.value);
			}

			//TODO Remove this when texture loading is fixed
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

			Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "GL window attributes loaded");

			///Create window
			window = SDL_CreateWindow(settings->windowName.c_str(), settings->windowX, settings->windowY, settings->windowWidth, settings->windowHeight, settings->windowFlags);
			if (!window) {
				Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Could not create window");
				return DarknecShutdown(3);
			}

			///Attempt to set default window icon
			if (settings->windowIcon != NULL) {
				SDL_Surface* icon = IMG_Load("DarknecIcon.png");
				if (icon) {
					SDL_SetWindowIcon(window, icon);
				}
				else {
					Darknec::logger("DarknecEngine", LogLevel::LOG_WARN, "Could not load default icon. Reverting to OS.");
				}
			}

			Darknec::logger("DarknecEngine", LogLevel::LOG_INFO, "Window created with flag : %i", settings->windowFlags);

			delete settings;

			///Create GL context
			glContext = SDL_GL_CreateContext(window);


			if (glContext == NULL) {
				Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Unable to create a GL context");
				return DarknecShutdown(4);
			}
			else {
				glewExperimental = GL_TRUE;
				int glewError = glewInit();
				if (glewError != GLEW_OK) {
					Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Could not initialise GLEW. Error: %s", glewGetErrorString(glewError));
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


					Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "GL Context set up successfully");
					Darknec::logger("DarknecEngine", LogLevel::LOG_INFO, "GPU vendor: %s", glGetString(GL_VENDOR));
					Darknec::logger("DarknecEngine", LogLevel::LOG_INFO, "Using OpenGL specification version: %s %s ", glGetString(GL_VERSION), contextString.c_str());
					Darknec::logger("DarknecEngine", LogLevel::LOG_INFO, "Using GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));


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


				if (Darknec::Callback::getRenderCallback() != NULL) {
					Darknec::Callback::getRenderCallback()(window);
				}
				else {
					Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Illegal State: NULL render method is not supported. Define a render method");
					DarknecShutdown(6);
				}
			}
		}
	}
}

/**
* @namespace Darknec
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

		Darknec::logger("DarknecEngine", LogLevel::LOG_SECTION, "============-START-=============");
		Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "Darknec starting up");

		int success = Darknec::detail::InitEngine(settings);
		if (success != 0) {
			Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "FATAL ERROR. Darknec could not initialise. Exit code: %i", success);
			DarknecShutdown(success);
		}

		Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "Darknec started successfully");

		Darknec::logger("DarknecEngine", LogLevel::LOG_SECTION, "======-END DARKNEC SETUP-=======");
		Darknec::logger("DarknecEngine", LogLevel::LOG_SECTION, "==========-START GAME-==========");
		if (Darknec::Callback::getInitCallback() != NULL) {
			Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "Running userdef init");
			Darknec::Callback::getInitCallback()(argc, argv, window, glContext);
		}
		else {
			Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Illegal State: NULL init method is not supported. Define an init method");
			DarknecShutdown(5);
		}

		Darknec::detail::GameLoopBegin();
	}

	/**
	* DarknecInit
	* @brief Shuts down DarknecEngine
	*
	* Shutdown DarknecEngine with a close code.
	*
	* @param close Code to quit with
	* @return always 0. Satisfies main()'s return value. Ensures or at least puts off tampering of state in main().
	*/
	int DarknecShutdown(int close) {
		//Got past engine initialisation
		if (close > 4) {
			if (Darknec::Callback::getCleanupCallback() != NULL) {
				Darknec::Callback::getCleanupCallback()();
			}
		}

		if (glContext != NULL) {
			SDL_GL_DeleteContext(glContext);
		}

		if (window != NULL) {
			SDL_DestroyWindow(window);
		}

		//SDL_IMG was initialised during setup and can be shutdown
		if (SDLIMGOK) {
			IMG_Quit();
		}

		//SDL was initialised during setup and can be shutdown
		if (SDLOK) {
			SDL_Quit();
		}


		Darknec::logger("DarknecEngine", LogLevel::LOG_SECTION, "=============-END-==============");
		
		if (close != 0) {
			Darknec::RUNSTATE = Darknec::CRASHED;
			exit(close);
		}

		//Return something for main(). Otherwise unnecessary. Ensures or at least puts off tampering of state in main();
		return 0; 
	}
}
