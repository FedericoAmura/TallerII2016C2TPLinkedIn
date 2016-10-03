/*
 * DataBasesHandler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_DB_HANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_DB_HANDLER_H_

#include <string>
#include "../json11/json11.hpp"
#include "../common/Exceptions.h"

class DB_Handler {
public:
	DB_Handler();

	bool validateUserPass(std::string user, std::string pass);

	std::string generateToken(std::string user, std::string pass);

	void registerNewUser(json11::Json jsonNewUser);

	virtual ~DB_Handler();
};

#endif /* APPSEVER_INCLUDE_HANDLERS_DATABASESHANDLER_H_ */
