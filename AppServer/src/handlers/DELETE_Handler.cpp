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
	// /users/<user_id2>/notif/<user_id2>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID1 = std::stoi(vec_uri[1]);
	uint32_t userID2 = std::stoi(vec_uri[3]);
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[Error] Token not found. User unauthorized. Reject contact request failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[Error] Invalid token. Non existent token. Reject contact request failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[Error] Invalid token. Token has expired. Reject contact request failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->declinarPeticion(userID1, userID2);
	} catch (NonexistentRequest &e) {
		std::cout << "[Error] Nonexistent Request. Reject contact request failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}

	return http_response("{}", STATUS_NO_CONTENT);
}

http_response DELETE_Handler::handle_delete_contact() {
	// /users/<user_id2>/contacts/<user_id2>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID1 = std::stoi(vec_uri[1]);
	uint32_t userID2 = std::stoi(vec_uri[3]);
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[Error] Token not found. User unauthorized. Reject contact request failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[Error] Invalid token. Non existent token. Reject contact request failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[Error] Invalid token. Token has expired. Reject contact request failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->eliminarContacto(userID1, userID2);
	} catch (NonexistentContact &e) {
		std::cout << "[Error] Nonexistent Contact. Delete contact failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}

	return http_response("{}", STATUS_NO_CONTENT);
}

DELETE_Handler::~DELETE_Handler() {
}
