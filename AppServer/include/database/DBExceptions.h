#ifndef APPSERVER_INCLUDE_DATABASE_DBEXCEPTIONS_H_
#define APPSERVER_INCLUDE_DATABASE_DBEXCEPTIONS_H_

#include <exception>

class LevelDBException: public std::runtime_error {
public:
 LevelDBException(const std::string what_arg) : std::runtime_error(what_arg) {}
};

class BadInputException: public std::runtime_error {
public:
	BadInputException(const std::string what_arg) : std::runtime_error(what_arg) {}
};

#endif /* APPSERVER_INCLUDE_DATABASE_DBEXCEPTIONS_H_ */
