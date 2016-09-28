/*
 * RequestHandler.cpp
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#include <iostream>
#include "../../include/handlers/RequestHandler.h"
#include "../../include/handlers/SharedServerHandler.h"

RequestHandler::RequestHandler(http_request* req) : request(req) {
}

void RequestHandler::sendReply(std::string res, int status){
	ns_printf(request->connection, "HTTP/1.1 %d\r\n"
							   	   "Content-Type: application/json\r\n"
							   	   "Content-Length: %d\r\n"
							   	   "\r\n"
							   	   "%s",
								   status, res.size(), res.c_str());
}

void RequestHandler::sendBadRequest(){
	sendReply("Bad Request\n", STATUS_BAD_REQUEST);
}

void RequestHandler::handle() {
	/* handling request */
	switch (validate_uri(request->uri())){
		case _LOGIN:
			handleLogIn();
			break;
		case _SIGNUP:
			handleSignUp();
			break;
		case _RECOMMEND:
			handleRecommendations();
			break;
		case _PROFESSIONALS:
			handleProfessionals();
			break;
		case _POPULAR:
			handlePopular();
			break;
		case _PROFILE:
			handleProfile();
			break;
		case _ADD_CONTACT:
			handleAddContact();
			break;
		case _CHAT:
			handleChat();
			break;
		default:
			sendBadRequest();
			break;
	}

	closeConnection();

	/* TODO mejorar lo de abajo, quitar*/
/*
	SharedServerHandler* sharedHandler = new SharedServerHandler(request);
	sharedHandler->start();
	sharedHandler->join();
	std::cout << request->method() << " " << request->uri() << std::endl;

	http_message* reply = sharedHandler->getReply();
	if (reply){
		std::cout << "status : " << reply->resp_code << std::endl;
		std::cout << "length : " << reply->body.len << std::endl;
		std::cout << "body : " << reply->body.p << std::endl;

		std::string res(reply->body.p, reply->body.len);
		int status = reply->resp_code;
		sendReply(res, status);
		memset((void*)reply->body.p, 0, reply->body.len);
	}
	std::cout << "request: processed " << std::endl;
	std::cout << std::endl;
	delete sharedHandler;
*/
}

void RequestHandler::closeConnection(){
	request->connection->flags |= NSF_SEND_AND_CLOSE;
}

bool RequestHandler::validToken(){
	return true;
}

void RequestHandler::handleLogIn(){
	sendReply("LogIn OK\n", STATUS_OK);
}

void RequestHandler::handleSignUp(){
	sendReply("SignUp OK\n", STATUS_CREATED);
}

void RequestHandler::handleRecommendations(){
	sendReply("Recommend OK\n", STATUS_OK);
}

void RequestHandler::handleProfessionals(){
	sendReply("Professionals OK\n", STATUS_OK);
}

void RequestHandler::handleProfile(){
	sendReply("Profile OK\n", STATUS_OK);
}

void RequestHandler::handlePopular(){
	sendReply("Popular OK\n", STATUS_OK);
}

void RequestHandler::handleAddContact(){
	sendReply("Add Contact OK\n", STATUS_OK);
}

void RequestHandler::handleChat(){
	sendReply("Chat OK\n", STATUS_OK);
}

RequestHandler::~RequestHandler() {
}
