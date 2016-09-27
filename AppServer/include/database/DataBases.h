/*
 * DataBases.h
 *
 *  Created on: Sep 27, 2016
 *      Author: emanuel
 */

#ifndef APP_SERVER_INCLUDE_DATABASE_DATABASES_H_
#define APP_SERVER_INCLUDE_DATABASE_DATABASES_H_

#include <iostream>

class DataBases {
private:
	std::string data;

public:
	DataBases();

	std::string toString();

	virtual ~DataBases();
};

#endif /* DATABASE_DATABASES_H_ */
