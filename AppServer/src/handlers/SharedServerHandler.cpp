/*
 * SharedServerHandler.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#include <iostream>
#include "../../include/handlers/SharedServerHandler.h"
#include "../../include/server/API_Server.h"

using std::cout;

/* communication between appserver and sharedserver*/
static void event_handler(struct ns_connection* c, int event, void* data) {
	  struct http_message* message = (struct http_message*) data;

	  switch (event) {
	  	  case NS_CONNECT:
	  		  std::cout << "....communicating with SharedServer...." << std::endl;
	  		  break;
	  	  case NS_HTTP_REPLY:
	  		  reply = message;
	  		  std::cout << "reply: " << message->body.p << std::endl;
	  		  processed_request = true;
	  		  c->flags |= NSF_SEND_AND_CLOSE;
	  		  break;
	      default:
		  	  break;
	  }
}

SharedServerHandler::SharedServerHandler(http_request* request) {
	ns_mgr_init(&mgr, NULL);
	std::string uri;
	uri.assign(request->message->uri.p, request->message->uri.len);
	std::string request_url = SHARED_SERVER_URL;
	request_url += uri;
	struct ns_connection* c = ns_connect_http(&mgr, event_handler,
					request_url.c_str(), NULL, NULL);
	if (!c)
		return;
	processed_request = false;
	reply = NULL;
}

void SharedServerHandler::run() {
	while (!processed_request) {
		ns_mgr_poll(&mgr, 1000);
	}
}

http_message* SharedServerHandler::getReply() {
	return reply;
}

SharedServerHandler::~SharedServerHandler() {
	ns_mgr_free(&mgr);
}

