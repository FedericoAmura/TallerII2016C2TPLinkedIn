/*
 * DataBasesHandler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_DATABASESHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_DATABASESHANDLER_H_

#include <iostream>

class DataBasesHandler {
public:
	DataBasesHandler();

	std::string toString();

	virtual ~DataBasesHandler();
};

#endif /* APPSEVER_INCLUDE_HANDLERS_DATABASESHANDLER_H_ */
