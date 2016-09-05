/*
 * ConnectionsHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef HANDLERS_CONNECTIONSHANDLER_H_
#define HANDLERS_CONNECTIONSHANDLER_H_

#include "../common/Thread.h"
#include "../fossa/fossa.h"

class ConnectionsHandler : public Thread{
private:
	struct ns_mgr mgr;
	struct ns_connection* connection;
	bool running;

public:
	ConnectionsHandler();

	bool isRunning();

	void run();

	void stop();

	virtual ~ConnectionsHandler();
};

#endif /* HANDLERS_CONNECTIONSHANDLER_H_ */
