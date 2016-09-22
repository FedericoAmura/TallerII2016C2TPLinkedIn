/*
 * RequestHandler.h
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_

#include "../common/req_res.h"

class RequestHandler{
 private:
	http_request* request;

	void sendReply(std::string res, int status, std::string content_type);
	void sendBadRequest();

 public:
	explicit RequestHandler(http_request* req);
	void handle();
	virtual ~RequestHandler();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_
