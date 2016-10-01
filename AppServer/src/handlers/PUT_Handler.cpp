/*
 * PUT_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/PUT_Handler.h"

PUT_Handler::PUT_Handler(http_request* req) : HTTPRequestHandler(req) {
}

void PUT_Handler::handleRequest() {
	switch (uri){
		case _USERS:
			/* update profile */
			handleUsers();
			break;
		case _JOB_POSITIONS:
			handleJobPositions();
			break;
		case _SKILLS:
			handleSkills();
			break;
		default:
			sendReply("", STATUS_BAD_REQUEST);
			break;
	}
}

void PUT_Handler::handleUsers() {
	sendReply("{\"msg\":\"Users\"}", STATUS_OK);
}

void PUT_Handler::handleJobPositions() {
	sendReply("{\"msg\":\"Positions\"}", STATUS_OK);
}

void PUT_Handler::handleSkills() {
	sendReply("{\"msg\":\"Skills\"}", STATUS_OK);
}

PUT_Handler::~PUT_Handler() {
}

