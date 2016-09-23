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
}

void RequestHandler::sendReply(std::string res, int status, std::string content_type){
	ns_printf(request->connection, "HTTP/1.1 %d\r\n"
							   	   "Content-Type: %s\r\n"
							   	   "Content-Length: %d\r\n"
							   	   "\r\n"
							   	   "%s",
								   status, content_type.c_str(), res.size(), res.c_str());
}

void RequestHandler::sendBadRequest(){
	sendReply("Bad Request\n", STATUS_BAD_REQUEST, CONTENT_TYPE_TEXT_PLAIN);
}

void RequestHandler::handle() {
	if (!validate_uri(request->uri())){
		sendBadRequest();
		return;
	}
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
		sendReply(res, status, CONTENT_TYPE_JSON);
		memset((void*)reply->body.p, 0, reply->body.len);
	}
	std::cout << "request: processed " << std::endl;
	std::cout << std::endl;
	delete sharedHandler;
}

RequestHandler::~RequestHandler() {
}

