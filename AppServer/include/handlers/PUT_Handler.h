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
	http_response handleUsers();
	http_response handleJobPositions();
	http_response handleSkills();

public:
	PUT_Handler(http_request* req);

	http_response handleRequest();

	virtual ~PUT_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_PUTHANDLER_H_ */
