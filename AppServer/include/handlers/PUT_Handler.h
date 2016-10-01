/*
 * PUT_Handler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_PUTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_PUTHANDLER_H_

#include "HTTPRequestHandler.h"

class PUT_Handler : public HTTPRequestHandler {
private:
	void handleUsers();
	void handleJobPositions();
	void handleSkills();

public:
	PUT_Handler(http_request* req);

	void handleRequest();

	virtual ~PUT_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_PUTHANDLER_H_ */
