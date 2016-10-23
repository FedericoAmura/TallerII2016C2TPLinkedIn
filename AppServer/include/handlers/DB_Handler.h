/*
 * DataBasesHandler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_DB_HANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_DB_HANDLER_H_

#include <string>
#include "../database/DBRaw.h"
#include "../database/DBChat.h"
#include "../common/Exceptions.h"
#include "../users/User.h"
#include "../json11/json11.hpp"

class DB_Handler {
private:
	DBRaw* db_users = 0;
	DBChat* db_chats = 0;

public:
	DB_Handler();

	bool validateUserPass(std::string user, std::string pass);

	bool validateTokenAndUserID(std::string token, std::string userID);

	json11::Json generateJsonWithTokenAndUserID(std:: string username, std::string password);

	void registerNewUser(struct user_record user);

	virtual ~DB_Handler();
};

#endif /* APPSEVER_INCLUDE_HANDLERS_DATABASESHANDLER_H_ */
