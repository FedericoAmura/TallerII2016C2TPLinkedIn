/*
 * ConnectionsHandler.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#include <iostream>
#include "../../include/handlers/ConnectionsHandler.h"
#include "../../include/server/API_Server.h"
#include "../../include/handlers/RequestHandler.h"

/* communication between clientapp and appserver*/
static void event_handler(struct ns_connection* c, int event, void* data) {
	  struct http_message* message = (struct http_message*) data;

	  switch (event) {
	  	  case NS_CONNECT:
	  		  std::cout << "Advice connected." << std::endl;
	  		  break;
	  	  case NS_HTTP_REQUEST:
	  	  {
	  		  DataBases* db = (DataBases*)c->user_data;
	  		  std::cout << "DataBases : " << db->toString() <<std::endl;
	  		  http_request request(c, message);
	  		  std::cout << "URI: " << request.uri() <<std::endl;
	  		  RequestHandler req_handler(&request);
	  		  req_handler.handle();
	  	  }
	  		  break;
	      default:
		  	  break;
	  }
}

ConnectionsHandler::ConnectionsHandler(DataBases* db) {
	ns_mgr_init(&mgr, NULL);
	running = false;
	connection = ns_bind(&mgr, DEFAULT_PORT, event_handler);
	if (!connection)
		return;
	ns_set_protocol_http_websocket(connection);
	// ns_enable_multithreading(connection);		// TODO
	running = true;
	connection->user_data = db;
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

