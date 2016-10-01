/*
 * DataBasesHandler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_DB_HANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_DB_HANDLER_H_

#include <iostream>

class DB_Handler {
public:
	DB_Handler();

	std::string toString();

	virtual ~DB_Handler();
};

#endif /* APPSEVER_INCLUDE_HANDLERS_DATABASESHANDLER_H_ */
