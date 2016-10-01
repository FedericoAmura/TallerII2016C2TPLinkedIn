/*
 * POST_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/POST_Handler.h"

POST_Handler::POST_Handler(http_request* req) : HTTPRequestHandler(req) {
}

void POST_Handler::handleRequest() {
	switch (uri){
		case _LOGIN:
			handleLogIn();
			break;
		case _SIGNUP:
			handleSignUp();
			break;
		case _RECOMMEND:
			handleRecommendations();
			break;
		case _ADD_CONTACT:
			handleAddContact();
			break;
		case _CHAT:
			handleChat();
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

void POST_Handler::handleLogIn() {
	std::string username, password;
	bool parsed = HttpParser::parse_username_password(request->message, username, password, 0);

	if (!parsed){
		std::cout << "LogIn failed" << std::endl;
		sendReply("", 409);
		return;
	}
	std::string token = db_handler->toString();
	std::string msg = "{\"token\":\"" + token + "\"}";
	sendReply(msg, STATUS_OK);
}

void POST_Handler::handleSignUp() {
	std::string username, password;
	bool parsed = HttpParser::parse_username_password(request->message, username, password, 1);

	if (!parsed){
		std::cout << "SignUp failed" << std::endl;
		sendReply("", 409);
		return;
	}
	std::string token = db_handler->toString();
	std::string msg = "{\"token\":\"" + token + "\"}";
	sendReply(msg, STATUS_CREATED);
}

void POST_Handler::handleRecommendations() {
	sendReply("{\"msg\":\"Recommendations\"}", STATUS_CREATED);
}

void POST_Handler::handleAddContact() {
	sendReply("{\"msg\":\"AddContact\"}", STATUS_CREATED);
}

void POST_Handler::handleChat() {
	sendReply("{\"msg\":\"Chat\"}", STATUS_CREATED);
}

void POST_Handler::handleJobPositions() {
	sendReply("{\"msg\":\"JobPositions\"}", STATUS_CREATED);
}

void POST_Handler::handleSkills() {
	sendReply("{\"msg\":\"Skills\"}", STATUS_CREATED);
}

void POST_Handler::handleCategories() {
	sendReply("{\"msg\":\"Categories\"}", STATUS_CREATED);
}

POST_Handler::~POST_Handler() {
}

