/*
 * ConnectionsHandler.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/ConnectionsHandler.h"
#include "../../include/server/API_Server.h"
#include "../../include/handlers/RequestHandler.h"
#include <iostream>

/* communication between appclient and appserver*/
static void event_handler(struct ns_connection* c, int event, void* data){
	  struct http_message* message = (struct http_message*) data;

	  switch (event){
	  	  case NS_CONNECT:
	  		  std::cout << "Advice connected." << std::endl;
	  		  break;
	  	  case NS_HTTP_REQUEST:
	  	  {
	  		  http_request request(c, message);
	  		  RequestHandler req_handler(&request);
	  		  req_handler.process();
	  	  }
	  		  break;
	      default:
		  	  break;
	  }
}

ConnectionsHandler::ConnectionsHandler() {
	ns_mgr_init(&mgr, NULL);
	running = false;
	connection = ns_bind(&mgr, DEFAULT_PORT, event_handler);
	if (!connection)
		return;
	ns_set_protocol_http_websocket(connection);
	//ns_enable_multithreading(connection);		// TODO
	running = true;
}

bool ConnectionsHandler::isRunning(){
	return running;
}

void ConnectionsHandler::run(){
	while(running){
		ns_mgr_poll(&mgr, 1000);
	}
}

void ConnectionsHandler::stop(){
	running = false;
}

ConnectionsHandler::~ConnectionsHandler() {
	ns_mgr_free(&mgr);
}

