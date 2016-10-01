/*
 * SharedServerHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERHANDLER_H_

#include "../common/req_res.h"

class SharedServerHandler {
private:
	http_request* request;

 public:
	explicit SharedServerHandler(http_request* req);

	virtual ~SharedServerHandler();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERHANDLER_H_
