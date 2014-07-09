#include <LoggingManager.h>
#include <stdarg.h>
#include <iostream>
#include <time.h>
#include <fstream>

void Logger::operator()(const char* format, ...) {;
	va_list varArgs;
	va_start(varArgs, format);
	manager->WINDOWSlog(this->logColours, LogLevel::LOG_DEBUG, this->ownerName.c_str(), format, varArgs);
	va_end(varArgs);
}

void Logger::operator()(LogLevel level, const char* format, ...) {
	va_list varArgs;
	va_start(varArgs, format);
	manager->WINDOWSlog(this->logColours, level, this->ownerName.c_str(), format, varArgs);
	va_end(varArgs);
}


LoggingManager::LoggingManager() {}

LoggingManager::LoggingManager(const char* file) {
	this->file = file;
}

void LoggingManager::setFile(const char* file) {
	this->file = file;
}

Logger LoggingManager::getLogger(const char* owner) {
	Logger loggerOwner;
	loggerOwner.logColours.push_back(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	loggerOwner.logColours.push_back(FOREGROUND_GREEN | FOREGROUND_BLUE);
	loggerOwner.logColours.push_back(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	loggerOwner.logColours.push_back(FOREGROUND_RED | FOREGROUND_INTENSITY);
	loggerOwner.logColours.push_back(BACKGROUND_RED);
	loggerOwner.logColours.push_back(FOREGROUND_GREEN);
	loggerOwner.logColours.push_back(FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);

	loggerOwner.manager = this;
	loggerOwner.ownerName = owner;
	return loggerOwner;

}

void LoggingManager::WINDOWSlog(std::vector<unsigned short> logColours, LogLevel level, const char* ownername, const char* format, va_list varargs) {
	HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE); //Console Handle
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hstdout, &csbi);

	char buffer[8192];
	vsnprintf_s(buffer, 8192, format, varargs);

	std::string text = formatString(level, ownername, buffer) + "\n";

	if (file != "") {
		std::ofstream file(file, std::ios::app);
		file << text;
		file.close();
	}

	SetConsoleTextAttribute(hstdout, logColours[level]);
	std::cout << text.c_str();

	#ifdef _DEBUG
		OutputDebugString(text.c_str());
	#endif




	SetConsoleTextAttribute(hstdout, csbi.wAttributes);
}

std::string LoggingManager::formatString(LogLevel level, const char* ownername, char* buffer) {
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
	return " " + levelString + "	" + timeString + "[" + ownername + "]" + bufferString;
}