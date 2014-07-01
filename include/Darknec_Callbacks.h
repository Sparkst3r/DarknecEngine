#ifndef DARKNEC_DARKNEC_CALLBACKS_H
#define DARKNEC_DARKNEC_CALLBACKS_H

#include <vector>
#include <SDL.h>



namespace Darknec {

	namespace Callback {

		#pragma region Settings

		struct WindowAttribute {
			WindowAttribute(SDL_GLattr attr, int value) {
				this->attr = attr;
				this->value = value;
			}
			SDL_GLattr attr;
			int value;
		};

		struct Settings {
			Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
			std::string windowName = "Darknec Engine Window";

			std::vector<WindowAttribute> attributes;

			int windowWidth = 640;
			int windowHeight = 480;

			int windowX = SDL_WINDOWPOS_CENTERED;
			int windowY = SDL_WINDOWPOS_CENTERED;

			SDL_Surface* windowIcon;

			
		};

#pragma endregion

		/* Callback for init */
		typedef void(*InitCallback) (int, char**, SDL_Window*, SDL_GLContext);
		void RegisterInitCallback(InitCallback callback);
		InitCallback getInitCallback();
		
		/* Callback for resize event */
		typedef void(*EventCallback) (SDL_Event);
		void RegisterEventCallback(EventCallback callback);
		EventCallback getEventCallback();

		/* Callback for resize event */
		typedef void(*ResizeCallback) (SDL_Event);
		void RegisterResizeCallback(ResizeCallback callback);
		ResizeCallback getResizeCallback();

		/* Callback for logic event */
		typedef void(*LogicCallback) ();
		void RegisterLogicCallback(LogicCallback callback);
		LogicCallback getLogicCallback();

		/* Callback for render event */
		typedef void(*RenderCallback) (SDL_Window*);
		void RegisterRenderCallback(RenderCallback callback);
		RenderCallback getRenderCallback();

		/* Callback for cleanup */
		typedef void(*CleanupCallback) ();
		void RegisterCleanupCallback(CleanupCallback callback);
		CleanupCallback getCleanupCallback();

	}
}

#endif