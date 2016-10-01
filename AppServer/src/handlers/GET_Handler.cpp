/*
 * GETHandler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/GET_Handler.h"

GET_Handler::GET_Handler(http_request* req) : HTTPRequestHandler(req) {
}

void GET_Handler::handleRequest() {
	switch (uri){
		case _USERS:
			// view profile "/users/<user_id>"
			handleUsers();
			break;
		case _PROFESSIONALS:
			handleProfessionals();
			break;
		case _POPULAR:
			handlePopular();
			break;
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

void GET_Handler::handleUsers() {
	sendReply("{\"msg\":\"Users\"}", STATUS_OK);
}

void GET_Handler::handleProfessionals() {
	sendReply("{\"msg\":\"Prefessionals\"}", STATUS_OK);
}

void GET_Handler::handlePopular() {
	sendReply("{\"msg\":\"Popular\"}", STATUS_OK);
}

void GET_Handler::handleJobPositions() {
	sendReply("{\"msg\":\"JobPositions\"}", STATUS_OK);
}

void GET_Handler::handleSkills() {
	sendReply("{\"msg\":\"Skills\"}", STATUS_OK);
}

void GET_Handler::handleCategories() {
	sendReply("{\"msg\":\"Categories\"}", STATUS_OK);
}

GET_Handler::~GET_Handler() {
}

