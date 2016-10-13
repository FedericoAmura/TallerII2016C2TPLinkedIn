/*
 * DataBasesHandler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/DB_Handler.h"

DB_Handler::DB_Handler() {
}

bool DB_Handler::validateUserPass(std::string user, std::string pass) {
	return true;
}

bool DB_Handler::validateTokenAndUserID(std::string token, std::string userID) {
	return true;
}

json11::Json DB_Handler::generateJsonWithTokenAndUserID(std:: string username, std::string password) {
	json11::Json json = json11::Json::object {
		{"userID" , username+password},
		{"token" , password+username},
	};
	return json;
}

void DB_Handler::registerNewUser(struct user_record user) {
	//throw ExistingUserException();
}

DB_Handler::~DB_Handler() {
}

