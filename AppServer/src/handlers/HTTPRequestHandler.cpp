/*
 * HTTPRequestHandler.cpp
 *
 *  Created on: Oct 1, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/HTTPRequestHandler.h"

HTTPRequestHandler::HTTPRequestHandler(http_request* req){
	this->request = req;
	this->db_json = (DBJSON*)req->connection->user_data;
	this->uri = validate_uri(req->uri());
}

void HTTPRequestHandler::sendReply(http_response* res) {
	ns_printf(request->connection, "HTTP/1.1 %d\r\n"
								   "Content-Type: application/json\r\n"
								   "Content-Length: %d\r\n"
								   "\r\n"
								   "%s",
								   res->res_code, res->message.size(), res->message.c_str());
}

void HTTPRequestHandler::closeConnection() {
	request->connection->flags |= NSF_SEND_AND_CLOSE;
}
