/*
 * DELETE_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/DELETE_Handler.h"
#include <iostream>

DELETE_Handler::DELETE_Handler(http_request* req) : HTTPRequestHandler(req) {
}

http_response DELETE_Handler::handleRequest() {
	http_response res;
	switch (uri){
		case _LOGIN:
			// /login
			res = handle_logout();
			break;
		case _USERS_REQ_CONTACT:
			// /users/<user_id2>/notif/<user_id2>
			res = handle_reject_contact_request();
			break;
		case _USERS_ADM_CONTACTS:
			// /users/<user_id1>/contacts/<user_id>
			res = handle_delete_contact();
			break;
		default:
			std::cout << "[Error] Method Not Allowed" << std::endl;
			return http_response("", STATUS_MET_NOT_ALLOWED);
			break;
	}
	return res;
}

http_response DELETE_Handler::handle_logout() {
	// /login
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[Error] Token not found. User unauthorized. Logout failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[Error] Invalid token. Non existent token. Logout failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[Error] Invalid token. Token has expired. Logout failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	db_json->logout(token);
	
	return http_response("{}", STATUS_NO_CONTENT);
}

http_response DELETE_Handler::handle_reject_contact_request() {
	return http_response("{}", STATUS_NO_CONTENT);
}

http_response DELETE_Handler::handle_delete_contact() {
	return http_response("{}", STATUS_NO_CONTENT);
}

DELETE_Handler::~DELETE_Handler() {
}
