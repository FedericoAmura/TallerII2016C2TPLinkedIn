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

class CurlInitException : public std::exception {
public:
	CurlInitException(){};
};

class CurlGetException : public std::exception {
public:
	CurlGetException(){};
};

class RequestRejected : public std::exception {
public:
	RequestRejected(){};
};

#endif /* APPSERVER_INCLUDE_COMMON_EXCEPTIONS_H_ */
