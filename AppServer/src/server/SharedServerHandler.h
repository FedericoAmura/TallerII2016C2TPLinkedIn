/*
 * SharedServerHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef SERVER_SHAREDSERVERHANDLER_H_
#define SERVER_SHAREDSERVERHANDLER_H_

#include "../common/Thread.h"
#include "../mongoose/mongoose.h"

static bool connected = false;

class SharedServerHandler: public Thread {
private:
	struct mg_mgr mgr;

public:
	SharedServerHandler();

	bool isConnected();

	void run();

	virtual ~SharedServerHandler();
};

#endif /* SERVER_SHAREDSERVERHANDLER_H_ */
