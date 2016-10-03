/*
 * PUT_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/PUT_Handler.h"

PUT_Handler::PUT_Handler(http_request* req) : HTTPRequestHandler(req) {
}

http_response PUT_Handler::handleRequest() {
	http_response res;
	switch (uri){
		case _USERS:
			/* update profile */
			res = handleUsers();
			break;
		case _JOB_POSITIONS:
			res = handleJobPositions();
			break;
		case _SKILLS:
			res = handleSkills();
			break;
		default:
			return http_response("", STATUS_BAD_REQUEST);
			break;
	}
	return res;
}

http_response PUT_Handler::handleUsers() {
	return http_response("{\"msg\":\"Users\"}", STATUS_OK);
}

http_response PUT_Handler::handleJobPositions() {
	return http_response("{\"msg\":\"Positions\"}", STATUS_OK);
}

http_response PUT_Handler::handleSkills() {
	return http_response("{\"msg\":\"Skills\"}", STATUS_OK);
}

PUT_Handler::~PUT_Handler() {
}

