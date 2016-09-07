/*
 * RequestHandler.cpp
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/RequestHandler.h"
#include "../../include/handlers/SharedServerHandler.h"
#include <iostream>

RequestHandler::RequestHandler(http_request* req) : request(req) {
}

void RequestHandler::process(){
	SharedServerHandler* sharedHandler = new SharedServerHandler(request);
	sharedHandler->start();
	sharedHandler->join();
	std::string uri;
	uri.assign(request->message->uri.p, request->message->uri.len);
	std::cout << "URI : " << uri << std::endl;
	http_message* reply = sharedHandler->getReply();
	if (reply)
		ns_printf(request->connection, "HTTP/1.1 200 OK\r\n"
	              	  	  	  	  	   "Content-Type: application/json\r\n"
	              	  	  	  	       "Content-Length: %d\r\n"
	              	  	  	  	  	   "\r\n"
									   "%s",
									   (int) strlen(reply->body.p), reply->body.p);

	else
		ns_printf(request->connection, "HTTP/1.1 404 ERROR\r\n"
									   "Content-Type: application/json\r\n"
									   "Content-Length: %d\r\n"
									   "\r\n"
									   "%s",
									   (int) strlen("Not Found"), "Not Found");

	std::cout<<"request: processed "<<std::endl;
	std::cout<<std::endl;
	delete sharedHandler;
}

RequestHandler::~RequestHandler() {
}

