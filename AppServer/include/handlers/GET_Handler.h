/*
 * GET_Handler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_GET_HANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_GET_HANDLER_H_

#include "HTTPRequestHandler.h"

class GET_Handler : public HTTPRequestHandler {
private:
	void handleUsers();
	void handleProfessionals();
	void handlePopular();
	void handleJobPositions();
	void handleSkills();
	void handleCategories();

public:
	GET_Handler(http_request* request);

	void handleRequest();

	virtual ~GET_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_GETHANDLER_H_ */
