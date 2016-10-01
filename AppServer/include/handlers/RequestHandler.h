/*
 * RequestHandler.h
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_

#include "../handlers/GET_Handler.h"
#include "../handlers/POST_Handler.h"
#include "../handlers/PUT_Handler.h"
#include "../handlers/DELETE_Handler.h"

class RequestHandler : HTTPRequestHandler {
 private:
	HTTPRequestHandler* http_handler;

 public:
	explicit RequestHandler(http_request* req);

	void handleRequest();

	virtual ~RequestHandler();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_
