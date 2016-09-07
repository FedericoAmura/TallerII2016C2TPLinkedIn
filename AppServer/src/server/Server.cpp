/*
 * Server.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#include "../../include/server/Server.h"
#include <iostream>

static void* serverHandler(void* arg){
	ConnectionsHandler* c = (ConnectionsHandler*) arg;
	char input = ' ';
	while (input == ' '){
		std::cin >> input;
	}
	c->stop();
	return NULL;
}

Server::Server() {
	connectionsHandler = new ConnectionsHandler();
}

bool Server::settting_ok(){
	return connectionsHandler->isRunning();
}

Server::~Server() {
	delete connectionsHandler;
}

void Server::run(){
	std::cout<<"...starting server..."<<std::endl;
	connectionsHandler->start();
	pthread_t t_handler;
	pthread_create(&t_handler, NULL, serverHandler, connectionsHandler);
	pthread_join(t_handler, NULL);
	connectionsHandler->join();
	std::cout<<"...shutting down server..."<<std::endl;
	sleep(3);
}
