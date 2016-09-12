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

void RequestHandler::sendReply(http_response* res){
	ns_printf(res->connection, "HTTP/1.1 %d\r\n"
							   "Content-Type: application/json\r\n"
							   "Content-Length: %d\r\n"
							   "\r\n"
							   "%s",
							   res->status(), res->contentLength(), res->body());
}

void RequestHandler::sendBadRequest(){
	ns_printf(request->connection, "HTTP/1.1 %d\r\n"
								   "Content-Type: text/plain\r\n"
							   	   "Content-Length: %d\r\n"
							   	   "\r\n"
							   	   "%s",
								   404, sizeof("Not Found"), "Not Found");
}

void RequestHandler::process() {
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
		http_response res(request->connection, reply);
		sendReply(&res);
	}
	std::cout << "request: processed " << std::endl;
	std::cout << std::endl;
	delete sharedHandler;
}

RequestHandler::~RequestHandler() {
}

