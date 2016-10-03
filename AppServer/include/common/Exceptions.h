/*
 * Exceptions.h
 *
 *  Created on: Oct 2, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_COMMON_EXCEPTIONS_H_
#define APPSERVER_INCLUDE_COMMON_EXCEPTIONS_H_

#include <exception>

class InvalidJsonException : public std::exception {
public:
	InvalidJsonException(){};
};




#endif /* APPSERVER_INCLUDE_COMMON_EXCEPTIONS_H_ */
