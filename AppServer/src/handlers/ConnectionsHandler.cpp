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
	  		//  std::cout << "\n" << MethodtoString(request.method()) << "\t" << request.uri() + request.query_string() <<std::endl;
			  Logger::log(INFO, MethodtoString(request.method()) + " \t" + request.uri() + request.query_string());
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
	connection = mg_bind(&mgr, DEFAULT_PORT, event_handler);
	if (!connection)
		return;
	mg_set_protocol_http_websocket(connection);
	mg_enable_multithreading(connection);
	running = true;
	connection->user_data = db_json;
	signal(SIGINT, SIGINT_Handler);
}

bool ConnectionsHandler::initialized() {
	return running;
}

void ConnectionsHandler::run() {
	while(running) {
		mg_mgr_poll(&mgr, 1000);
	}
}

ConnectionsHandler::~ConnectionsHandler() {
	mg_mgr_free(&mgr);
}
