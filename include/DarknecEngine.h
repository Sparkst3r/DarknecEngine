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

namespace Darknec {
	
	///Exposed function
	//Start engine
	//	$settings	: Settings structure
	//	$argc		: Argument count
	//	$argv		: Argument values
	void DarknecInit(Darknec::Callback::Settings* settings, int argc, char *argv[]);
	
	///Exposed function
	//Shutdown engine.
	//	$close		: Close code to shutdown with
	int DarknecShutdown(int close = 0);

}


#endif;