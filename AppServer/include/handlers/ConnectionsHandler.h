/*
 * ConnectionsHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_

#include "../common/Thread.h"
#include "../../include/handlers/RequestHandler.h"

class ConnectionsHandler : public Thread {
 private:
	struct mg_mgr mgr;
	struct mg_connection* connection;
	bool running;

 public:
	ConnectionsHandler(DBJSON* db_json);
	bool isRunning();
	void run();
	void stop();
	virtual ~ConnectionsHandler();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_
