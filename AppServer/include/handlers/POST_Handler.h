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
	void handleLogIn();
	void handleSignUp();
	void handleUsers();
	void handleRecommendations();
	void handleAddContact();
	void handleChat();
	void handleJobPositions();
	void handleSkills();
	void handleCategories();

public:
	POST_Handler(http_request* req);

	void handleRequest();

	virtual ~POST_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_POSTHANDLER_H_ */
