/*
 * ConnectionsHandler.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#include <iostream>
#include "../../include/handlers/ConnectionsHandler.h"

/* communication between clientapp and appserver*/
static void event_handler(struct ns_connection* c, int event, void* data) {
	  struct http_message* message = (struct http_message*) data;

	  switch (event) {
	  	  case NS_HTTP_REQUEST:
	  	  {
	  		  http_request request(c, message);
	  		  std::cout << "URI: "<< request.uri() <<std::endl;
	  		  RequestHandler req_handler(&request);
	  		  req_handler.handleRequest();
	  	  }
	  		  break;
	      default:
		  	  break;
	  }
}

ConnectionsHandler::ConnectionsHandler(DB_Handler* db_handler) {
	ns_mgr_init(&mgr, NULL);
	running = false;
	connection = ns_bind(&mgr, DEFAULT_PORT, event_handler);
	if (!connection)
		return;
	ns_set_protocol_http_websocket(connection);
	// ns_enable_multithreading(connection);		// TODO
	running = true;
	connection->user_data = db_handler;
}

bool ConnectionsHandler::isRunning() {
	return running;
}

void ConnectionsHandler::run() {
	while(running) {
		ns_mgr_poll(&mgr, 1000);
	}
}

void ConnectionsHandler::stop() {
	running = false;
}

ConnectionsHandler::~ConnectionsHandler() {
	ns_mgr_free(&mgr);
}

