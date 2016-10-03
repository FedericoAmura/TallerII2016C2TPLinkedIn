/*
 * POST_Handler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_POSTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_POSTHANDLER_H_

#include "HTTPRequestHandler.h"

class POST_Handler : public HTTPRequestHandler {
private:
	http_response handleLogIn();
	http_response handleSignUp();
	http_response handleUsers();
	http_response handleRecommendations();
	http_response handleAddContact();
	http_response handleChat();
	http_response handleJobPositions();
	http_response handleSkills();
	http_response handleCategories();

public:
	POST_Handler(http_request* req);

	http_response handleRequest();

	virtual ~POST_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_POSTHANDLER_H_ */
