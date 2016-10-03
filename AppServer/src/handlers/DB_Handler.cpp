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

std::string DB_Handler::generateToken(std::string user, std::string pass) {
	return (user + pass);
}

void DB_Handler::registerNewUser(json11::Json json) {
	//throw InvalidJsonException();
}

DB_Handler::~DB_Handler() {
}

