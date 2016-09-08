/*
 * SharedServerHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERHANDLER_H_

#include "../common/Thread.h"
#include "../fossa/fossa.h"
#include "../common/req_res.h"

static struct http_message* reply = NULL;
static bool processed_request = false;

class SharedServerHandler: public Thread {
 private:
	struct ns_mgr mgr;

 public:
	explicit SharedServerHandler(http_request* request);
	void run();
	http_message* getReply();
	virtual ~SharedServerHandler();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_SHAREDSERVERHANDLER_H_
