/*
 * Server.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#include "Server.h"

Server::Server() {
	sharedServerHandler = new SharedServerHandler();
	connectionsHandler = new ConnectionsHandler();
}

bool Server::settting_ok(){
	return (sharedServerHandler->isConnected() && connectionsHandler->isRunning());
}

Server::~Server() {
	delete sharedServerHandler;
	delete connectionsHandler;
}

void Server::run(){
	sharedServerHandler->start();
	connectionsHandler->start();

	sharedServerHandler->join();
	connectionsHandler->join();
}
