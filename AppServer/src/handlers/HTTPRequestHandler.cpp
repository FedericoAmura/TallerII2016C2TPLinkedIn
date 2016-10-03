/*
 * HTTPRequestHandler.cpp
 *
 *  Created on: Oct 1, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/HTTPRequestHandler.h"

HTTPRequestHandler::HTTPRequestHandler(http_request* req){
	this->request = req;
	this->db_handler = (DB_Handler*)req->connection->user_data;
	this->uri = validate_uri(req->uri());
}

bool HTTPRequestHandler::validToken() {
	std::string token = "";
	bool parsed = HttpParser::parse_token(request->message, token);
	if (!parsed)
		return false;
	/* TODO validar token en la base de datos */
	return true;
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
