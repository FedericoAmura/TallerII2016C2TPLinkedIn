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
	mg_printf(request->connection, "HTTP/1.1 %d\r\n"
								   "Content-Type: application/json\r\n"
								   "Content-Length: %d\r\n"
								   "\r\n"
								   "%s",
								   (unsigned)res->res_code, (unsigned)res->message.size(), res->message.c_str());
	if (res->message.size() < 30)
		Logger::log(INFO, "Status Code: " + std::to_string(res->res_code) + ". Response: " + res->message);
	else {
		std::string json = res->message;
		Logger::log(INFO, "Status Code: " + std::to_string(res->res_code) + ". Response: " + json.substr(0,29) + "...");
	}
}

void HTTPRequestHandler::closeConnection() {
	request->connection->flags |= MG_F_SEND_AND_CLOSE;
}
