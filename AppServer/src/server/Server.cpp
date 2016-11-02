/*
 * Server.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#include "../../include/server/Server.h"

Server::Server() {
	try {
		db_json = new DBJSON(new DBRaw("/tmp/db_users", &std::cout));
	} catch (LevelDBException &e) {
		init_ok = false;
		return;
	}
	connectionsHandler = new ConnectionsHandler(db_json);
	init_ok = true;
}

bool Server::settting_ok() {
	return init_ok && connectionsHandler->initialized();
}

Server::~Server() {
	if (connectionsHandler != 0)
		delete connectionsHandler;
	if (db_json != 0)
		delete db_json;
}

void Server::run() {
	std::cout << "...starting server on port 8888..." << std::endl;
	std::cout << "...to stop the server press Ctrl+C ..." << std::endl;
	connectionsHandler->run();
	std::cout << "...shutting down server..." << std::endl;
	sleep(2);
}
