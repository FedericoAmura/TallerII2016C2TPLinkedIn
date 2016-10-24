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

http_response RequestHandler::handleRequest() {
	if (uri == _INVALID_URI) {
		std::cout << "ERROR >> Invalid uri " << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}

	http_response res;

	METHOD method = request->method();
	bool method_ok = true;
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
			http_handler = new DELETE_Handler(request);
			break;
		default:
			method_ok = false;
			res = http_response("", STATUS_MET_NOT_ALLOWED);
			break;
	}

	if (method_ok)
		res = http_handler->handleRequest();

	return res;
}

RequestHandler::~RequestHandler() {
	if (http_handler)
		delete http_handler;
}
