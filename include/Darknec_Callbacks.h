#ifndef DARKNEC_DARKNEC_CALLBACKS_H
#define DARKNEC_DARKNEC_CALLBACKS_H

#include <Core.h>

namespace Darknec {

	namespace Callback {

		#pragma region Settings

		struct WindowAttribute {
			WindowAttribute(long attr, std::string value) {
				this->attr = attr;
				this->value = value;
			}
			long attr;
			std::string value;
		};

		struct Settings {
			//Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
			std::string windowName = "Darknec Engine Window";

			std::vector<WindowAttribute> attributes;
			//std::vector<WindowFlag> flags;

			int windowWidth = 640;
			int windowHeight = 480;

			int windowX = 0;
			int windowY = 0;

			//SDL_Surface* windowIcon;

			
		};

#pragma endregion

		/* Callback for init */
		typedef void(*InitCallback) (int, char**, GLFWwindow*);
		void RegisterInitCallback(InitCallback callback);
		InitCallback getInitCallback();
		
		///* Callback for resize event */
		//typedef void(*EventCallback) (SDL_Event);
		//void RegisterEventCallback(EventCallback callback);
		//EventCallback getEventCallback();

		///* Callback for resize event */
		//typedef void(*ResizeCallback) (SDL_Event);
		//void RegisterResizeCallback(ResizeCallback callback);
		//ResizeCallback getResizeCallback();

		/* Callback for logic event */
		typedef void(*LogicCallback) ();
		void RegisterLogicCallback(LogicCallback callback);
		LogicCallback getLogicCallback();

		/* Callback for render event */
		typedef void(*RenderCallback) (GLFWwindow*);
		void RegisterRenderCallback(RenderCallback callback);
		RenderCallback getRenderCallback();

		/* Callback for cleanup */
		typedef void(*CleanupCallback) ();
		void RegisterCleanupCallback(CleanupCallback callback);
		CleanupCallback getCleanupCallback();

	}
}

#endif