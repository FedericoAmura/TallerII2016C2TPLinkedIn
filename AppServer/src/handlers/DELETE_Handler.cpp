/*
 * DELETE_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/DELETE_Handler.h"

DELETE_Handler::DELETE_Handler(http_request* req) : HTTPRequestHandler(req) {
}

void DELETE_Handler::handleRequest() {
	switch (uri){
		case _JOB_POSITIONS:
			handleJobPositions();
			break;
		case _SKILLS:
			handleSkills();
			break;
		case _CATEGORIES:
			handleCategories();
			break;
		default:
			sendReply("", STATUS_BAD_REQUEST);
			break;
	}
}

void DELETE_Handler::handleJobPositions() {
	sendReply("", STATUS_NO_CONTENT);
}

void DELETE_Handler::handleSkills() {
	sendReply("", STATUS_NO_CONTENT);
}

void DELETE_Handler::handleCategories() {
	sendReply("", STATUS_NO_CONTENT);
}

DELETE_Handler::~DELETE_Handler() {
}

