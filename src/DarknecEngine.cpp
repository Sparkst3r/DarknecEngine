/**
* @file DarknecEngine.cpp
* Engine core
*/

#include <DarknecEngine.h>
#include <State.h>

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
	namespace Detail {
		
		/**
		* loadSettings
		* Load vital settings from Settings.xml.
		*
		* @return settings struct containing all the settings loaded.
		*/
		Darknec::Callback::Settings* loadSettings() {
			using namespace rapidxml;

			std::hash_map<std::string, SDL_WindowFlags> flagMap;
			flagMap["WINDOW_SHOWN"] = SDL_WindowFlags::SDL_WINDOW_SHOWN;
			flagMap["WINDOW_RESIZABLE"] = SDL_WindowFlags::SDL_WINDOW_RESIZABLE;

			std::hash_map<std::string, SDL_GLattr> attrMap;
			attrMap["GL_RED_SIZE"] = SDL_GLattr::SDL_GL_RED_SIZE;
			attrMap["GL_GREEN_SIZE"] = SDL_GLattr::SDL_GL_GREEN_SIZE;
			attrMap["GL_BLUE_SIZE"] = SDL_GLattr::SDL_GL_BLUE_SIZE;
			attrMap["GL_ALPHA_SIZE"] = SDL_GLattr::SDL_GL_ALPHA_SIZE;
			attrMap["GL_BUFFER_SIZE"] = SDL_GLattr::SDL_GL_BUFFER_SIZE;
			attrMap["GL_DOUBLEBUFFER"] = SDL_GLattr::SDL_GL_DOUBLEBUFFER;
			attrMap["GL_STENCIL_SIZE"] = SDL_GLattr::SDL_GL_STENCIL_SIZE;
			attrMap["GL_CONTEXT_MAJOR"] = SDL_GLattr::SDL_GL_CONTEXT_MAJOR_VERSION;
			attrMap["GL_CONTEXT_MINOR"] = SDL_GLattr::SDL_GL_CONTEXT_MINOR_VERSION;
			attrMap["GL_DEPTH_SIZE"] = SDL_GLattr::SDL_GL_DEPTH_SIZE;

			

			std::ifstream settingsStream = std::ifstream("Settings.xml");
			if (settingsStream.good()) {
				Darknec::Callback::Settings* settings = new Darknec::Callback::Settings();
				Uint32 windowFlags = SDL_WindowFlags::SDL_WINDOW_OPENGL;


				xml_document<> doc;
				xml_node<>* root_node;

				std::stringstream stringStream;
				stringStream << settingsStream.rdbuf();
				std::string xmlDataString = stringStream.str();
				std::vector<char> data(xmlDataString.begin(), xmlDataString.end());
				data.push_back('\0');
				doc.parse<0>(&data[0]);

				root_node = doc.first_node("Settings"); //Root node

				for (xml_node<>* settingsIter = root_node->first_node(); settingsIter; settingsIter = settingsIter->next_sibling()) {

					if (std::string(settingsIter->name()) == std::string("Window")) {
						for (xml_node<>* windowIter = settingsIter->first_node(); windowIter; windowIter = windowIter->next_sibling()) {
							if (std::string(windowIter->name()) == std::string("WindowName")) {
								settings->windowName = std::string(windowIter->value());
							}
							else if (std::string(windowIter->name()) == std::string("WindowWidth")) {
								settings->windowWidth = atoi(std::string(windowIter->value()).c_str());
							}
							else if (std::string(windowIter->name()) == std::string("WindowHeight")) {
								settings->windowHeight = atoi(std::string(windowIter->value()).c_str());
							}
							else if (std::string(windowIter->name()) == std::string("WindowX")) {
								if (std::string(windowIter->value()) == std::string("CENTRE") || std::string(windowIter->value()) == std::string("CENTER")) {
									settings->windowX = SDL_WINDOWPOS_CENTERED;
								}
								else {
									settings->windowX = atoi(std::string(windowIter->value()).c_str());
								}
							}
							else if (std::string(windowIter->name()) == std::string("WindowY")) {
								if (std::string(windowIter->value()) == std::string("CENTRE") || std::string(windowIter->value()) == std::string("CENTER")) {
									settings->windowY = SDL_WINDOWPOS_CENTERED;
								}
								else {
									settings->windowY = atoi(std::string(windowIter->value()).c_str());
								}
							}
							else if (std::string(windowIter->name()) == std::string("WindowAttribute")) {
								xml_attribute<char>* attrName = windowIter->first_attribute();
								std::string str = std::string(attrName->value());
								if (!str.empty()) {
									SDL_GLattr glAttr = attrMap[str];
									Darknec::logger("Settings Loader", LogLevel::LOG_LOG, "Window Attribute: '%s' set to %s", str.c_str(), std::string(windowIter->value()).c_str());
									settings->attributes.push_back(Darknec::Callback::WindowAttribute(glAttr, atoi(std::string(windowIter->value()).c_str())));
								}
							}
							else if (std::string(windowIter->name()) == std::string("WindowFlag")) {
								std::string str = std::string(windowIter->value());
								Darknec::logger("Settings Loader", LogLevel::LOG_LOG, "Window Flag: %s.", str.c_str());
								windowFlags |= flagMap[str];
							}
						}
					}
				}

				settings->windowFlags = windowFlags;

				return settings;
			}
			else {
				Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Settings.xml doesn't exist in local directory. Please provide a Settings.xml");
				DarknecShutdown(3);
			}
			return NULL;
		}

		/**
		* InitEngine
		* Setup engine libs and state
		*
		* @param settings settings for engine
		* @return error state. Set to 0 if setup is successful
		*/
		int InitEngine() {
			Darknec::Callback::Settings* settings = loadSettings();

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
	void DarknecInit(int argc, char* argv[]) {

		Darknec::logger("DarknecEngine", LogLevel::LOG_SECTION, "============-START-=============");
		Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "Darknec starting up");

		int success = Darknec::Detail::InitEngine();
		if (success != 0) {
			Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "FATAL ERROR. Darknec could not initialise. Exit code: %i", success);
			DarknecShutdown(success);
		}

		Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "Darknec started successfully");

		Darknec::logger("DarknecEngine", LogLevel::LOG_SECTION, "======-END DARKNEC SETUP-=======");
		Darknec::logger("DarknecEngine", LogLevel::LOG_SECTION, "==========-START GAME-==========");
		if (Darknec::Callback::getInitCallback() != NULL) {
			Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "Running userdef init");
			Darknec::Callback::getInitCallback()(argc, argv, Darknec::Detail::window, Darknec::Detail::glContext);
		}
		else {
			Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Illegal State: NULL init method is not supported. Define an init method");
			DarknecShutdown(5);
		}



		Darknec::Detail::GameLoopBegin();
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

		if (Darknec::Detail::glContext != NULL) {
			SDL_GL_DeleteContext(Darknec::Detail::glContext);
		}

		if (Darknec::Detail::window != NULL) {
			SDL_DestroyWindow(Darknec::Detail::window);
		}

		//SDL_IMG was initialised during setup and can be shutdown
		if (Darknec::Detail::SDLIMGOK) {
			IMG_Quit();
		}

		//SDL was initialised during setup and can be shutdown
		if (Darknec::Detail::SDLOK) {
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
