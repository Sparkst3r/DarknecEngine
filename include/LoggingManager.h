#ifndef DARKNEC_LOGGINGMANAGER_H
#define DARKNEC_LOGGINGMANAGER_H

#include <string>
#include <vector>
#include <windows.h>

//Log severity
//	$LOG_LOG	: Lowest level log. Use for indicating important changes in the game.
//	$LOG_INFO	: Used for printing informative data out.
//	$LOG_WARN	: Print a warning message if something isn't as it should be.
//	$LOG_ERROR	: Use if something is badly wrong and the program is likely to be unstable.
//	$LOG_FATAL	: The program must exit immediately
//	$LOG_DEBUG	: General purpose debug output for development.
//	$LOG_SECTION: Cosmetic log output. Defines sections of program flow such as "Engine start" or "Engine quit"
enum LogLevel {
	LOG_LOG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL,
	LOG_DEBUG,
	LOG_SECTION,
};

class LoggingManager; //Forward declare LoggingManager

class Logger {
public:
	//Instance of the logger's logging manager
	LoggingManager* manager;

	//Name of logger
	std::string ownerName;
	std::vector<unsigned short> logColours;

	//Formatted log with default logging level
	//	$format	: Text format
	//	$varargs: Data
	void operator()(const char* format, ...);

	//Formatted log with logging level
	//	$level	: Logging level
	//	$format	: Text format
	//	$varargs: Data
	void operator()(LogLevel level, const char* format, ...);
};

class LoggingManager {
	friend class Logger;

public:
	LoggingManager();
	LoggingManager(const char* file);

	Logger getLogger(const char* owner);

	void setFile(const char* file);

private:

	std::string file = "";

	//
	std::string formatString(LogLevel level, const char* ownername, char* buffer);

	//Windows log function
	//	$logColours	: Colours with which to print log string
	//	$level		: Logging level
	//	$ownername	: Name of logger
	//	$format		: Text format
	//	$varargs	: Data
	void WINDOWSlog(std::vector<unsigned short> logColours, LogLevel level, const char* ownername, const char* format, va_list varargs);
	
	//UNIX log function
	//	$logColours	: Colours with which to print log string
	//	$level		: Logging level
	//	$ownername	: Name of logger
	//	$format		: Text format
	//	$varargs	: Data
	void UNIXlog(std::vector<unsigned short> logColours, LogLevel level, const char* ownername, const char* format, va_list varargs);
};



#endif