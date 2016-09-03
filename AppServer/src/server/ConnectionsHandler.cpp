/*
 * ConnectionsHandler.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#include "ConnectionsHandler.h"
#include <iostream>

/* communication between appclient and appserver*/
static void event_handler(struct mg_connection* c, int event, void* p){
	  if (event == MG_EV_HTTP_REQUEST) {
	    struct http_message *hm = (struct http_message *) p;

	    std::cout << "Android device connected. " <<std::endl;

	    mg_send_head(c, 200, hm->message.len, "Content-Type: text/plain");
	    mg_printf(c, "%.*s", hm->message.len, hm->message.p);
	  }
	  running = false;
}

ConnectionsHandler::ConnectionsHandler() {
	mg_mgr_init(&mgr, NULL);
	running = false;
	connection = mg_bind(&mgr, "8888", event_handler);
	if (!connection)
		return;
	mg_set_protocol_http_websocket(connection);
	running = true;
}

bool ConnectionsHandler::isRunning(){
	return running;
}

void ConnectionsHandler::run(){
	while(running){
		mg_mgr_poll(&mgr, 1000);
	}
}

ConnectionsHandler::~ConnectionsHandler() {
	mg_mgr_free(&mgr);
}

