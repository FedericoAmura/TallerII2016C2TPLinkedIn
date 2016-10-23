/*
 * Server.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#include "../../include/server/Server.h"
#include <iostream>

static void* serverHandler(void* arg) {
	ConnectionsHandler* c = reinterpret_cast<ConnectionsHandler*>(arg);
	char input = ' ';
	while (input == ' ') {
		std::cin >> input;
	}
	c->stop();
	return NULL;
}

Server::Server() {
	try {
		db_handler = new DB_Handler();
	} catch (LevelDBException &e) {
		init_ok = false;
		return;
	}
	connectionsHandler = new ConnectionsHandler(db_handler);
	init_ok = true;
}

bool Server::settting_ok() {
	return init_ok;
}

Server::~Server() {
	if (connectionsHandler != 0)
		delete connectionsHandler;
	if (db_handler != 0)
		delete db_handler;
}

void Server::run() {
	std::cout << "...starting server..." << std::endl;
	std::cout << "...to stop the server press a key..." << std::endl;
	connectionsHandler->start();
	pthread_t t_handler;
	pthread_create(&t_handler, NULL, serverHandler, connectionsHandler);
	pthread_join(t_handler, NULL);
	connectionsHandler->join();
	std::cout << "...shutting down server..." << std::endl;
	sleep(3);
}
