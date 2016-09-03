/*
 * SharedServerHandler.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#include "SharedServerHandler.h"
#include <iostream>

/* communication between appserver and sharedserver*/
static void event_handler(struct mg_connection* c, int event, void* p){
	  if (event == MG_EV_CONNECT) {
	    std::cout << "AppServer connected to SharedServer. " <<std::endl;
	  }

	  if (event == MG_EV_HTTP_REPLY){
		struct http_message *hm = (struct http_message *) p;
		std::cout << "reply: " << hm->body.p <<std::endl;
		connected = false;
	  }
}

SharedServerHandler::SharedServerHandler() {
	mg_mgr_init(&mgr, NULL);
	struct mg_connection* c = mg_connect_http(&mgr, event_handler, "http://localhost:8080/skills",NULL, NULL);
	if (!c)
		connected = false;
	connected = true;
}

bool SharedServerHandler::isConnected(){
	return connected;
}

void SharedServerHandler::run(){
	while (connected){
		mg_mgr_poll(&mgr, 1000);
	}
}

SharedServerHandler::~SharedServerHandler() {
	mg_mgr_free(&mgr);
}

