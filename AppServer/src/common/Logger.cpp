#include "../../include/common/Logger.h"

std::ofstream Logger::logfile;

void Logger::log(LogType type, std::string output) {
    time_t rawtime;
    std::string type_s = "";
    switch (type) {
        case ERROR:
            type_s = "ERROR";
            break;
        case WARN:
            type_s = "WARN";
            break;
        case INFO:
            type_s = "INFO";
            break;
        case DEBUG:
            type_s = "DEBUG";
            break;
        default:
            type_s = "???";
            break;
    }
    struct tm * timeinfo;
    char buffer[100];
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer,80,"%Y-%m-%d %I:%M:%S",timeinfo);
    std::string timestamp(buffer);
    logfile.open(LOGFILE, std::ofstream::app);
    std::string line = "[" + timestamp + "]" + '\t' + type_s + '\t' + output + '\n';
    logfile << line;//"[" << timestamp << "]" << '\t' << type_s << '\t' << output << '\n';
    std::cout << line << '\n';
    logfile.close();
}
