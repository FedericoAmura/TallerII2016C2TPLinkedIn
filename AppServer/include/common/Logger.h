#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <time.h>
#include <iostream>

enum LogType { 	ERROR = 1,
				WARN = 3,
				INFO = 7,
				DEBUG = 15 };

static const std::string LOGFILE = "appserver.log";

class Logger {
private:
    static std::ofstream logfile;
    static LogType logLevel;

public:
    static void log(LogType type, std::string output);
    static void setLevel(LogType type);
};

#endif /* LOGGER_H_ */
