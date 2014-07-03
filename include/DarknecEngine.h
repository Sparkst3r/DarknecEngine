#ifndef DARKNEC_DARKNECENGINE_H 
#define DARKNEC_DARKNECENGINE_H

#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <SDL.h>
#include <Darknec_Callbacks.h>
#include <State.h>

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
	void DarknecInit(Darknec::Callback::Settings* settings, int argc, char* argv[]);

	/**
	* DarknecInit
	* @brief Shuts down DarknecEngine
	*
	* Shutdown DarknecEngine with a close code.
	*
	* @param close Code to quit with
	*/
	int DarknecShutdown(int close = 0);

}
#endif // !DARKNEC_DARKNECENGINE_H
