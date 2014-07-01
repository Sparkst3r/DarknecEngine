#include <Darknec_Callbacks.h>

//Core Namepsace
namespace Darknec {

	//Callback functions. Interface between game and engine.
	namespace Callback {
		//
		InitCallback initCallback;
		void RegisterInitCallback(InitCallback callback) {
			initCallback = callback;
		}
	


		InitCallback getInitCallback() {
			return initCallback;
		}

		EventCallback eventCallback;
		void RegisterEventCallback(EventCallback callback) {
			eventCallback = callback;
		}
		EventCallback getEventCallback() {
			return eventCallback;
		}

		ResizeCallback resizeCallback;
		void RegisterResizeCallback(ResizeCallback callback) {
			resizeCallback = callback;
		}
		ResizeCallback getResizeCallback() {
			return resizeCallback;
		}

		LogicCallback logicCallback;
		void RegisterLogicCallback(LogicCallback callback) {
			logicCallback = callback;
		}
		LogicCallback getLogicCallback() {
			return logicCallback;
		}

		RenderCallback renderCallback;
		void RegisterRenderCallback(RenderCallback callback) {
			renderCallback = callback;
		}
		RenderCallback getRenderCallback() {
			return renderCallback;
		}

		CleanupCallback cleanupCallback;
		void RegisterCleanupCallback(CleanupCallback callback) {
			cleanupCallback = callback;
		}
		CleanupCallback getCleanupCallback() {
			return cleanupCallback;
		}


	}
}