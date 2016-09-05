/*
 * SharedServerHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef HANDLERS_SHAREDSERVERHANDLER_H_
#define HANDLERS_SHAREDSERVERHANDLER_H_

#include "../common/Thread.h"
#include "../fossa/fossa.h"
#include "../common/req_res.h"

static struct http_message* reply = NULL;
static bool processed_request = false;

class SharedServerHandler: public Thread {
private:
	struct ns_mgr mgr;

public:
	SharedServerHandler(http_request* request);

	void run();

	http_message* getReply();

	virtual ~SharedServerHandler();
};

#endif /* HANDLERS_SHAREDSERVERHANDLER_H_ */
