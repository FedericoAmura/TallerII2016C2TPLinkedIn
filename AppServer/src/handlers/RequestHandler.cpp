/*
 * RequestHandler.cpp
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/RequestHandler.h"

RequestHandler::RequestHandler(http_request* req) : HTTPRequestHandler(req) {
	http_handler = 0;
}

void RequestHandler::handleRequest() {
	METHOD method = request->method();
	bool uri_ok = true;
	switch (method){
		case _GET:
			http_handler = new GET_Handler(request);
			break;
		case _POST:
			http_handler = new POST_Handler(request);
			break;
		case _PUT:
			http_handler = new PUT_Handler(request);
			break;
		case _DELETE:
		{
			http_handler = new DELETE_Handler(request);
		}
			break;
		default:
			uri_ok = false;
			sendReply("", STATUS_BAD_REQUEST);
			break;
	}

	if (uri_ok)
		http_handler->handleRequest();

	closeConnection();
}

RequestHandler::~RequestHandler() {
	if (http_handler)
		delete http_handler;
}
