#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <time.h>
#include <iostream>

enum LogType { ERROR, WARN, INFO, DEBUG };

static const std::string LOGFILE = "appserver.log";

class Logger {
private:
    static std::ofstream logfile;

public:
    static void log(LogType type, std::string output);
};

#endif /* LOGGER_H_ */
