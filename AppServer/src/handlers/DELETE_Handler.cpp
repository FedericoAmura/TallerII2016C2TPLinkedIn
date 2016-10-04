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
			res = handleCloseSession();
			break;
		case _USERS_REQ_CONTACT:
			// /users/<user_id2>/notif/<user_id2>
			res = handleRejectContactRequest();
			break;
		case _USERS_ADM_CONTACTS:
			// /users/<user_id1>/contacts/<user_id>
			res = handleDeleteContact();
			break;
		default:
			std::cout << "ERROR >> Invalid uri " << std::endl;
			return http_response("", STATUS_BAD_REQUEST);
			break;
	}
	return res;
}

http_response DELETE_Handler::handleCloseSession() {
	return http_response("", STATUS_NO_CONTENT);
}

http_response DELETE_Handler::handleRejectContactRequest() {
	return http_response("", STATUS_NO_CONTENT);
}

http_response DELETE_Handler::handleDeleteContact() {
	return http_response("", STATUS_NO_CONTENT);
}

DELETE_Handler::~DELETE_Handler() {
}

