/**
* @file LoggingManager.h
* Manages logging to console and to file
*/

#ifndef DARKNEC_LOGGINGMANAGER_H 
#define DARKNEC_LOGGINGMANAGER_H
#include <Core.h>
#include <time.h>

namespace Darknec {

	/**
	* @enum LogLevel
	* @brief Log severity
	*
	* Defines levels of log priority
	*
	*/
	enum LogLevel {
		LOG_LOG, ///Lowest level log. Use for indicating general changes in the game.
		LOG_INFO, ///Used for printing informative data out.
		LOG_WARN, ///Print a warning message if something isn't as it should be but will not affect stability.
		LOG_ERROR, ///Use if something is badly wrong and the program is likely to be unstable.
		LOG_FATAL, ///The program must exit immediately. 
		LOG_DEBUG, ///General purpose debug output for development.
		LOG_SECTION, ///Cosmetic log output. Defines sections of program flow such as "Engine start" or "Engine quit"
	};

	/**
	* @class LoggingManager LoggingManager.h "include/LoggingManager.h"
	* @brief Logging system
	*
	* Manages logging to console and to file
	*
	*/
	class LoggingManager {
	public:

		/**
		* Logging to console manager
		* Does not write to file
		*/
		LoggingManager();

		/**
		* Logging to console and file manager
		* Writes to specified file
		* @param file file to write to. Appends to this file everytime the game is run.
		*/
		LoggingManager(const char* file);

		/**
		* Change file that the logger appends to
		* @param file file to change to.
		*/
		void setFile(const char* file);

		/**
		* Log formatted text to console and file
		* Log level set to LOG_DEBUG
		* @param format formatting pattern using type specifiers. @see std::printf
		* @param variadic data
		*/
		void operator()(const char* format, ...);

		/**
		* Log formatted text to console and file
		* @param level logging level
		* @param format formatting pattern using type specifiers. @see std::printf
		* @param variadic data
		*/
		void operator()(LogLevel level, const char* format, ...);

		/**
		* Log formatted text to console and file
		* @param owner tag log text with an owner.
		* @param level logging level
		* @param format formatting pattern using type specifiers. @see std::printf
		* @param variadic data
		*/
		void operator()(const char* owner, LogLevel level, const char* format, ...);

		void internalLog(const char* owner, LogLevel level, const char* format, va_list);

	private:
		///File to log to
		std::string file_ = "";

		///Colour bit to print loglevel in.
		std::vector<unsigned short> logColours_;

		/**
		* Format output text for printing to console and file
		*
		* @param owner name to use to identify where the log has been printed. eg. "DarknecEngine-Core" or "Achievement"
		* @param level the logging level to print text in.
		* @param buffer buffer containing pre formatted log text
		* @return the formatted string
		*/
		std::string formatString(const char* owner, LogLevel level, char* buffer);

		/**
		* Print log text to console and file for a windows environment/console
		*
		* @param owner name to use to identify where the log has been printed. eg. "DarknecEngine-Core" or "Achievement"
		* @param level the logging level to print text in.
		* @param format formatting pattern using type specifiers. @see std::printf
		* @param varargs data
		*/
		void WINDOWSlog(const char* owner, LogLevel level, const char* format, va_list varargs);

		/**
		* Print log text to console and file for a UNIX environment/console
		*
		* @param owner name to use to identify where the log has been printed. eg. "DarknecEngine-Core" or "Achievement"
		* @param level the logging level to print text in.
		* @param format formatting pattern using type specifiers. @see std::printf
		* @param varargs data
		*/
		void UNIXlog(const char* owner, LogLevel level, const char* format, va_list varargs);
	};
}

#endif // !DARKNEC_LOGGINGMANAGER_H
