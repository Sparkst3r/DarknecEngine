/**
* @file LoggingManager.h
* Manages logging to console and to file
*/

#include <LoggingManager.h>

#if defined(_WIN32)
#include <windows.h>
#endif

#include <State.h>



/**
* Logging to console manager
* Does not write to file
*/
Darknec::LoggingManager::LoggingManager() : LoggingManager("") {}

/**
* Logging to console and file manager
* Writes to specified file
* @param file file to write to. Appends to this file everytime the game is run.
*/
Darknec::LoggingManager::LoggingManager(const char* file) : file_(file) {
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE); //Console Handle

	COORD bufferSize;
	SMALL_RECT consSize;
	CONSOLE_SCREEN_BUFFER_INFO oldWindowInfo;
	GetConsoleScreenBufferInfo(hstdout, &oldWindowInfo);

	bufferSize.X = 120;
	bufferSize.Y = 1000;
	SetConsoleScreenBufferSize(hstdout, bufferSize);


	consSize.Left = 0;
	consSize.Top = 0;
	consSize.Right = 119;
	consSize.Bottom = 24;
	SetConsoleWindowInfo(hstdout, TRUE, &consSize);
	
	logColours_.push_back(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	logColours_.push_back(FOREGROUND_GREEN | FOREGROUND_BLUE);
	logColours_.push_back(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	logColours_.push_back(FOREGROUND_RED | FOREGROUND_INTENSITY);
	logColours_.push_back(BACKGROUND_RED);
	logColours_.push_back(FOREGROUND_GREEN);
	logColours_.push_back(FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
}

/**
* Change file that the logger appends to
* @param file file to change to.
*/
void Darknec::LoggingManager::setFile(const char* file) {
	this->file_ = file;
}

/**
* Log formatted text to console and file
* Log level set to LOG_DEBUG
* @param format formatting pattern using type specifiers. @see std::printf
* @param variadic data
*/
void Darknec::LoggingManager::operator()(const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->internalLog("", LogLevel::LOG_DEBUG, format, varArgs);
	va_end(varArgs);
}

/**
* Log formatted text to console and file
* @param level logging level
* @param format formatting pattern using type specifiers. @see std::printf
* @param variadic data
*/
void Darknec::LoggingManager::operator()(LogLevel level, const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->internalLog("", level, format, varArgs);
	va_end(varArgs);
}

/**
* Log formatted text to console and file
* @param owner tag log text with an owner.
* @param level logging level
* @param format formatting pattern using type specifiers. @see std::printf
* @param variadic data
*/
void Darknec::LoggingManager::operator()(const char* owner, LogLevel level, const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	this->internalLog(owner, level, format, varArgs);
	va_end(varArgs);
}

void Darknec::LoggingManager::internalLog(const char* owner, LogLevel level, const char* format, va_list varargs) {
	this->WINDOWSlog(owner, level, format, varargs);
}

/**
* Format output text for printing to console and file
*
* @param owner name to use to identify where the log has been printed. eg. "DarknecEngine-Core" or "Achievement"
* @param level the logging level to print text in.
* @param buffer buffer containing pre formatted log text
* @return the formatted string
*/
std::string Darknec::LoggingManager::formatString(const char* owner, LogLevel level, char* buffer) {
	//Get time
	tm timeStruct;
	time_t timet = time(0);
	localtime_s(&timeStruct, &timet);
	char timeString[30];
	strftime(timeString, sizeof(timeString), "[%Y-%m-%d %H:%M:%S]", &timeStruct);


	//Format logger level
	std::string levelString;
	switch (level) {
		case LOG_LOG:
			levelString = "LOG";
			break;
		case LOG_INFO:
			levelString = "INFO";
			break;
		case LOG_WARN:
			levelString = "WARN";
			break;
		case LOG_ERROR:
			levelString = "ERROR";
			break;
		case LOG_FATAL:
			levelString = "FATAL";
			break;
		case LOG_SECTION:
			levelString = "SECT";
			break;
		case LOG_DEBUG:
			levelString = "DEBUG";
			break;
	}

	//Concat output
	std::string bufferString(buffer);

	return " " + levelString + "	" + timeString + "[" + owner + "]" + bufferString;
}

/**
* Print log text to console and file for a windows environment/console
*
* @param owner name to use to identify where the log has been printed. eg. "DarknecEngine-Core" or "Achievement"
* @param level the logging level to print text in.
* @param format formatting pattern using type specifiers. @see std::printf
* @param varargs data
*/
void Darknec::LoggingManager::WINDOWSlog(const char* owner, LogLevel level, const char* format, va_list varargs) {
#if defined(_WIN32)
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE); //Console Handle
	CONSOLE_SCREEN_BUFFER_INFO prevStateInfo;
	GetConsoleScreenBufferInfo(hstdout, &prevStateInfo);

	char buffer[8192];
	vsnprintf_s(buffer, 8192, format, varargs);

	std::string text = formatString(owner, level, buffer) + "\n";

	if (this->file_ != "") {
		std::ofstream file(this->file_, std::ios::app);
		file << text;
		file.close();
	}

	SetConsoleTextAttribute(hstdout, logColours_[level]);
	std::cout << text.c_str();

	#if defined(_DEBUG)
	OutputDebugString(text.c_str());
	#endif

	SetConsoleTextAttribute(hstdout, prevStateInfo.wAttributes);
#endif

}
