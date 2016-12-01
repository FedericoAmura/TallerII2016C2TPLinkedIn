/*
 * Server.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#include "../../include/server/Server.h"

Server::Server(bool gcm_mode) {
	Logger::log(INFO, "Application Server");
	Logger::log(INFO, "init");
	try {
		db_json = new DBJSON(new DBRaw("/tmp/db_users", &std::cout), gcm_mode);
	} catch (LevelDBException &e) {
		Logger::log(ERROR, "Error in database initialization.");
		init_ok = false;
		return;
	} catch (const std::ios_base::failure &e) {
		std::cout << "[ERROR] Default photo for DBRaw not found. Check the path where the application is running." << std::endl;
		Logger::log(ERROR, "Default photo for DBRaw not found. Check the path where the application is running.");
		init_ok = false;
		return;
	}
	connectionsHandler = new ConnectionsHandler(db_json);
	init_ok = true;
}

bool Server::setting_ok() {
	return init_ok && connectionsHandler->initialized();
}

Server::~Server() {
	if (connectionsHandler != 0)
		delete connectionsHandler;
	if (db_json != 0)
		delete db_json;
}

void Server::run() {
	Logger::log(INFO, "...starting server on port 8888...");
	std::cout << "...starting server on port 8888..." << std::endl;
	std::cout << "...to stop the server press Ctrl+C ..." << std::endl;
	connectionsHandler->run();
	Logger::log(INFO, "...shutting down server...");
	std::cout << "...shutting down server..." << std::endl;
	sleep(2);
}
