/*
 * DataBasesHandler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_DB_HANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_DB_HANDLER_H_

#include <string>
#include "../common/Exceptions.h"
#include "../common/User.h"
#include "../json11/json11.hpp"

class DB_Handler {
public:
	DB_Handler();

	bool validateUserPass(std::string user, std::string pass);

	bool validateTokenAndUserID(std::string token, std::string userID);

	json11::Json generateJsonWithTokenAndUserID(std:: string username, std::string password);

	void registerNewUser(struct user_record user);

	virtual ~DB_Handler();
};

#endif /* APPSEVER_INCLUDE_HANDLERS_DATABASESHANDLER_H_ */
