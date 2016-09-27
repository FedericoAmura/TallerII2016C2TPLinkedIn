/*
 * ConnectionsHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_

#include "../common/Thread.h"
#include "../fossa/fossa.h"
#include "../database/DataBases.h"

class ConnectionsHandler : public Thread {
 private:
	struct ns_mgr mgr;
	struct ns_connection* connection;
	bool running;

 public:
	ConnectionsHandler(DataBases* db);
	bool isRunning();
	void run();
	void stop();
	virtual ~ConnectionsHandler();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_
