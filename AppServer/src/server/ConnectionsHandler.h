/*
 * ConnectionsHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef SERVER_CONNECTIONSHANDLER_H_
#define SERVER_CONNECTIONSHANDLER_H_

#include "../common/Thread.h"
#include "../mongoose/mongoose.h"

static bool running = false;

class ConnectionsHandler : public Thread{
private:
	struct mg_mgr mgr;
	struct mg_connection* connection;

public:
	ConnectionsHandler();

	bool isRunning();

	void run();

	virtual ~ConnectionsHandler();
};

#endif /* SERVER_CONNECTIONSHANDLER_H_ */
