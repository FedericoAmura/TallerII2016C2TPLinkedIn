/*
 * Server.h
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_SERVER_SERVER_H_
#define APPSERVER_INCLUDE_SERVER_SERVER_H_

#include "../handlers/ConnectionsHandler.h"

class Server {
 private:
	ConnectionsHandler* connectionsHandler;

 public:
	Server();
	bool settting_ok();
	void run();
	virtual ~Server();
};

#endif  // APPSERVER_INCLUDE_SERVER_SERVER_H_
