/*
 * ConnectionsHandler.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#include <iostream>
#include "../../include/handlers/ConnectionsHandler.h"

/* communication between clientapp and appserver*/
static void event_handler(struct mg_connection* c, int event, void* data) {
	  struct http_message* message = (struct http_message*) data;

	  switch (event) {
	  	  case MG_EV_HTTP_REQUEST:
	  	  {
	  		  http_request request(c, message);
	  		  std::cout << MethodtoString(request.method()) << "\t" << request.uri() + request.query_string() <<std::endl;
	  		  RequestHandler req_handler(&request);
	  		  http_response response = req_handler.handleRequest();
	  		  req_handler.sendReply(&response);
	  		  req_handler.closeConnection();
	  	  }
	  		  break;
	      default:
		  	  break;
	  }
}

ConnectionsHandler::ConnectionsHandler(DBJSON* db_json) {
	mg_mgr_init(&mgr, NULL);
	running = false;
	connection = mg_bind(&mgr, DEFAULT_PORT, event_handler);
	if (!connection)
		return;
	mg_set_protocol_http_websocket(connection);
	mg_enable_multithreading(connection);
	running = true;
	connection->user_data = db_json;
}

bool ConnectionsHandler::isRunning() {
	return running;
}

void ConnectionsHandler::run() {
	while(running) {
		mg_mgr_poll(&mgr, 500);
	}
}

void ConnectionsHandler::stop() {
	running = false;
}

ConnectionsHandler::~ConnectionsHandler() {
	mg_mgr_free(&mgr);
}
