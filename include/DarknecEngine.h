/**
* @file DarknecEngine.h
* Engine core header
*/

#ifndef DARKNEC_DARKNECENGINE_H 
#define DARKNEC_DARKNECENGINE_H

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
	void DarknecInit(int argc, char* argv[]);

	/**
	* DarknecInit
	* @brief Shuts down DarknecEngine
	*
	* Shutdown DarknecEngine with a close code.
	*
	* @param close Code to quit with
	* @return always 0. Satisfies main()'s return value. Ensures or at least puts off tampering of state in main().
	*/
	int DarknecShutdown(int close = 0);

}
#endif // !DARKNEC_DARKNECENGINE_H
