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
	uri = validate_uri(request->uri());
	db_handler = (DataBasesHandler*) request->connection->user_data;
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

void RequestHandler::handleRequest() {

	if (uri == _INVALID_URI){
		sendBadRequest();
		closeConnection();
		return;
	}

	METHOD method = request->method();
	switch (method){
		case _GET:
			handleGetRequest();
			break;
		case _POST:
			handlePostRequest();
			break;
		case _PUT:
			handlePutRequest();
			break;
		case _DELETE:
			handleDeleteRequest();
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

void RequestHandler::handleGetRequest(){
	switch (uri){
		case _USERS:
			// view profile "/users/<user_id>"
			handleUsers_GET();
			break;
		case _PROFESSIONALS:
			handleProfessionals_GET();
			break;
		case _POPULAR:
			handlePopular_GET();
			break;
		case _JOB_POSITIONS:
			handleJobPositions_GET();
			break;
		case _SKILLS:
			handleSkills_GET();
			break;
		case _CATEGORIES:
			handleCategories_GET();
			break;
		default:
			sendBadRequest();
			break;
	}
}
void RequestHandler::handlePostRequest(){
	switch (uri){
		case _USERS:
			// create account or login
			handleUsers_POST();
			break;
		case _JOB_POSITIONS:
			handleJobPositions_POST();
			break;
		case _SKILLS:
			handleSkills_POST();
			break;
		case _CATEGORIES:
			handleCategories_POST();
			break;
		default:
			sendBadRequest();
			break;
	}
}

void RequestHandler::handlePutRequest(){
	switch (uri){
		case _USERS:
			/* update profile */
			handleUsers_PUT();
			break;
		case _JOB_POSITIONS:
			handleJobPositions_PUT();
			break;
		case _SKILLS:
			handleSkills_PUT();
			break;
		default:
			sendBadRequest();
			break;
	}
}

void RequestHandler::handleDeleteRequest(){
	switch (uri){
		case _JOB_POSITIONS:
			handleJobPositions_DELETE();
			break;
		case _SKILLS:
			handleSkills_DELETE();
			break;
		case _CATEGORIES:
			handleCategories_DELETE();
			break;
		default:
			sendBadRequest();
			break;
	}
}

void RequestHandler::closeConnection(){
	request->connection->flags |= NSF_SEND_AND_CLOSE;
}

bool RequestHandler::validToken(){
	return true;
}

/***********************************************************************/
/******************************* GET ***********************************/
void RequestHandler::handleUsers_GET()			{sendReply("GET Users OK\n", STATUS_OK);}
void RequestHandler::handleProfessionals_GET()	{sendReply("GET Professionals OK\n", STATUS_OK);}
void RequestHandler::handlePopular_GET()		{sendReply("GET Popular OK\n", STATUS_OK);}
void RequestHandler::handleJobPositions_GET()	{sendReply("GET JobPositions OK\n", STATUS_OK);}
void RequestHandler::handleSkills_GET()			{sendReply("GET Skills OK\n", STATUS_OK);}
void RequestHandler::handleCategories_GET()		{sendReply("GET Categories_GET OK\n", STATUS_OK);}

/******************************* POST **********************************/
void RequestHandler::handleUsers_POST()			{sendReply("POST Users OK\n", STATUS_CREATED);}
void RequestHandler::handleRecommendations_POST(){sendReply("POST Recommendations OK\n", STATUS_CREATED);}
void RequestHandler::handleAddContact_POST()	{sendReply("POST Add Contact OK\n", STATUS_CREATED);}
void RequestHandler::handleChat_POST()			{sendReply("POST Chat OK\n", STATUS_CREATED);}
void RequestHandler::handleJobPositions_POST()	{sendReply("POST JobPositions OK\n", STATUS_CREATED);}
void RequestHandler::handleSkills_POST()		{sendReply("POST Skills OK\n", STATUS_CREATED);}
void RequestHandler::handleCategories_POST()	{sendReply("POST Categories OK\n", STATUS_CREATED);}

/******************************* PUT ***********************************/
void RequestHandler::handleUsers_PUT()			{sendReply("PUT Users OK\n", STATUS_OK);}
void RequestHandler::handleJobPositions_PUT()	{sendReply("PUT JobPositions OK\n", STATUS_OK);}
void RequestHandler::handleSkills_PUT()			{sendReply("PUT Skills OK\n", STATUS_OK);}

/******************************* DELETE ********************************/
void RequestHandler::handleJobPositions_DELETE(){sendReply("", STATUS_NO_CONTENT);}
void RequestHandler::handleSkills_DELETE()		{sendReply("", STATUS_NO_CONTENT);}
void RequestHandler::handleCategories_DELETE()	{sendReply("", STATUS_NO_CONTENT);}

/***********************************************************************/
/***********************************************************************/

void RequestHandler::handleLogIn(){
	sendReply("LogIn OK\n", STATUS_OK);
}

void RequestHandler::handleSignUp(){
	sendReply("SignUp OK\n", STATUS_CREATED);
}
RequestHandler::~RequestHandler() {
}
