/*
 * HTTPRequestHandler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_HTTPREQUESTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_HTTPREQUESTHANDLER_H_

#include "../common/req_res.h"
#include "../parsers/HttpParser.h"
#include "DB_Handler.h"

class HTTPRequestHandler {
protected:
	http_request* request;
	DB_Handler* db_handler;
	URI uri;

public:
	HTTPRequestHandler(http_request* req);

	virtual void handleRequest() = 0;

	void sendReply(std::string msg, int status);

	void closeConnection();

	virtual ~HTTPRequestHandler(){};
};



#endif /* APPSERVER_INCLUDE_HANDLERS_HTTPREQUESTHANDLER_H_ */
