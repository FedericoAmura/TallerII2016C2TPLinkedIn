/*
 * DELETE_Handler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_DELETEHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_DELETEHANDLER_H_

#include "HTTPRequestHandler.h"

class DELETE_Handler : public HTTPRequestHandler {
private:
	http_response handleJobPositions();
	http_response handleSkills();
	http_response handleCategories();

public:
	DELETE_Handler(http_request* req);

	http_response handleRequest();

	virtual ~DELETE_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_DELETEHANDLER_H_ */
