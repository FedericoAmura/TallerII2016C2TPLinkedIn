/*
 * Server.h
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "SharedServerHandler.h"
#include "ConnectionsHandler.h"

class Server {
private:
	SharedServerHandler* sharedServerHandler;
	ConnectionsHandler* connectionsHandler;

public:
	Server();

	bool settting_ok();

	void run();

	virtual ~Server();
};

#endif /* SERVER_H_ */
