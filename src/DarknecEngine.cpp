/**
* @file DarknecEngine.cpp
* Engine core
*/

#include <DarknecEngine.h>
#include <State.h>

//For temp hack
#include <system/ComponentSystem.h>
#include <system/GameObjectSystem.h>
#include <system/InputSystem.h>

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

			std::hash_map<std::string, long> glfwMap;
			glfwMap["GL_RED_BITS"] = GLFW_RED_BITS;
			glfwMap["GL_GREEN_BITS"] = GLFW_GREEN_BITS;
			glfwMap["GL_BLUE_BITS"] = GLFW_BLUE_BITS;
			glfwMap["GL_ALPHA_BITS"] = GLFW_ALPHA_BITS;
			glfwMap["GL_DEPTH_BITS"] = GLFW_DEPTH_BITS;
			glfwMap["GL_STENCIL_SIZE"] = GLFW_STENCIL_BITS;
			glfwMap["GL_CONTEXT_MAJOR"] = GLFW_CONTEXT_VERSION_MAJOR;
			glfwMap["GL_CONTEXT_MINOR"] = GLFW_CONTEXT_VERSION_MINOR;
			glfwMap["WINDOW_SHOWN"] = GLFW_VISIBLE;
			glfwMap["WINDOW_RESIZABLE"] = GLFW_RESIZABLE;

			

			std::ifstream settingsStream = std::ifstream("Settings.xml");
			if (settingsStream.good()) {
				Darknec::Callback::Settings* settings = new Darknec::Callback::Settings();

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
							/*else if (std::string(windowIter->name()) == std::string("WindowX")) {
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
							}*/
							else if (std::string(windowIter->name()) == std::string("WindowAttribute")) {
								xml_attribute<char>* attrName = windowIter->first_attribute();
								std::string str = std::string(attrName->value());
								if (!str.empty()) {
									long glAttr = glfwMap[str];
									Darknec::logger("Settings Loader", LogLevel::LOG_LOG, "Window Attribute: '%s' set to %s", str.c_str(), std::string(windowIter->value()).c_str());
									settings->attributes.push_back(Darknec::Callback::WindowAttribute(glAttr, std::string(windowIter->value()).c_str()));
								}
							}
							else if (std::string(windowIter->name()) == std::string("BaseAssetPath")) {
								std::string str = std::string(windowIter->value());
								Darknec::logger("Settings Loader", LogLevel::LOG_LOG, "Base Path: %s.", str.c_str());
								Darknec::baseAssetPath = str;
							}
						}
					}
				}

				return settings;
			}
			else {
				Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Settings.xml doesn't exist in local directory. Please provide a Settings.xml");
				DarknecShutdown(3);
			}
			return NULL;
		}

		void error_callback(int error, const char* description) {
			fputs(description, stderr);
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
			if (!glfwInit()) {
				Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Could not initialise GLFW");
				return DarknecShutdown(1);
			}
			else {
				Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "GLFW Initialised");
			}


			///Set GL attributes
			for (Darknec::Callback::WindowAttribute attr : settings->attributes) {
				//GL version
				if (attr.attr == GLFW_CONTEXT_VERSION_MAJOR ||
					attr.attr == GLFW_CONTEXT_VERSION_MINOR) {
					int version = atoi(attr.value.c_str());
					glfwWindowHint(attr.attr, version);
				}

				//Bools
				if (attr.attr == GLFW_RESIZABLE ||
					attr.attr == GLFW_VISIBLE || 
					attr.attr == GLFW_DECORATED ||
					attr.attr == GLFW_AUTO_ICONIFY ||
					attr.attr == GLFW_FLOATING ||
					attr.attr == GLFW_STEREO ||
					attr.attr == GLFW_SRGB_CAPABLE ||
					attr.attr == GLFW_DOUBLEBUFFER ||
					attr.attr == GLFW_OPENGL_FORWARD_COMPAT ||
					attr.attr == GLFW_OPENGL_DEBUG_CONTEXT) {
					bool boolean = attr.value == std::string("TRUE");
					glfwWindowHint(attr.attr, boolean);
				}

				//Bit depth
				if (attr.attr == GLFW_RED_BITS ||
					attr.attr == GLFW_GREEN_BITS ||
					attr.attr == GLFW_BLUE_BITS ||
					attr.attr == GLFW_ALPHA_BITS ||
					attr.attr == GLFW_DEPTH_BITS ||
					attr.attr == GLFW_STENCIL_BITS) {
					int bits = atoi(attr.value.c_str());
					glfwWindowHint(attr.attr, bits);
				}


			}

			Darknec::WindowHeight = settings->windowHeight;
			Darknec::WindowWidth = settings->windowWidth;

			glfwSetErrorCallback(error_callback);
			Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "GL window attributes loaded");

			///Create window
			window = glfwCreateWindow(settings->windowWidth, settings->windowHeight, settings->windowName.c_str(), NULL, NULL);
			if (!window) {
				Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Could not create window");
				return DarknecShutdown(3);
			}

			//Darknec::logger("DarknecEngine", LogLevel::LOG_INFO, "Window created with flag : %i", settings->windowFlags);

			delete settings;

			glfwSetKeyCallback(window, glfwInputKeyPressCallback);
			glfwSetCursorPosCallback(window, glfwInputMouseMoveCallback);
			
			///Create GL context
			glfwMakeContextCurrent(window);
			glfwSwapInterval(1);


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
				//SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &contextAttribute);
				//if (contextAttribute == SDL_GL_CONTEXT_PROFILE_COMPATIBILITY) {
				//	contextString = "compatibility";
				//}
				//else {
				//	contextString = "core";
				//}
				int glslVersion = GLVersion_MAJOR * 100 + GLVersion_MINOR * 10;

				switch (glslVersion) {
					case 200:
						glslVersion = 110;
					case 210:
						glslVersion = 120;
					case 300:
						glslVersion = 130;
					case 310:
						glslVersion = 140;
					case 320:
						glslVersion = 150;
				}

				Darknec::GLSLVersion = glslVersion;
				Darknec::logger("DarknecEngine", LogLevel::LOG_LOG, "GL Context set up successfully");
				Darknec::logger("DarknecEngine", LogLevel::LOG_INFO, "GPU vendor: %s", glGetString(GL_VENDOR));
				Darknec::logger("DarknecEngine", LogLevel::LOG_INFO, "Using OpenGL specification version: %s %s ", glGetString(GL_VERSION), contextString.c_str());
				Darknec::logger("DarknecEngine", LogLevel::LOG_INFO, "Using GLSL version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
			}

			RUNSTATE = PLAYING;
			return 0;
		}

		///Internal function
		//Enter game loop
		void GameLoopBegin() {
			while (Darknec::RUNSTATE != Darknec::STOPPED && Darknec::RUNSTATE != Darknec::CRASHED) {
				glfwPollEvents();
				if (glfwWindowShouldClose(Darknec::Detail::window)) {
					Darknec::RUNSTATE = Darknec::STOPPED;
				}


				sys4->pushUpdate();
				//if (Darknec::Callback::getLogicCallback() != NULL) {
				//	Darknec::Callback::getLogicCallback()();
				//}


				if (Darknec::Callback::getRenderCallback() != NULL) {
					Darknec::Callback::getRenderCallback()(window);
				}
				else {
					Darknec::logger("DarknecEngine", LogLevel::LOG_FATAL, "Illegal State: NULL render method is not supported. Define a render method");
					DarknecShutdown(6);
				}
				glfwSwapBuffers(window);
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
			Darknec::Callback::getInitCallback()(argc, argv, Darknec::Detail::window);
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
		//Shutdown componentsystem. Temp hack
		delete sys1;
		delete sys2;

		//Got past engine initialisation
		if (close > 4) {
			if (Darknec::Callback::getCleanupCallback() != NULL) {
				Darknec::Callback::getCleanupCallback()();
			}
		}


		//SDL was initialised during setup and can be shutdown
		if (Darknec::Detail::SDLOK) {
			glfwTerminate();
		}

		//sq_close(Darknec::squirrel);

		Darknec::logger("DarknecEngine", LogLevel::LOG_SECTION, "=============-END-==============");
		
		if (close != 0) {
			Darknec::RUNSTATE = Darknec::CRASHED;
			exit(close);
		}

		//Return something for main(). Otherwise unnecessary. Ensures or at least puts off tampering of state in main();
		return 0; 
	}
}
