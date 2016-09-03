/*
 * Server.h
 *
 *  Created on: Sep 1, 2016
 *      Author: emanuel
 */

#ifndef SERVER_H_
#define SERVER_H_

#include "../mongoose/mongoose.h"

static const char* ip_sharedServer = "http://localhost:8080/skills";
static const char* server_http_port = "8888";

static bool client_mode_running = false;
static bool server_mode_running = false;

class Server {
private:
	struct mg_mgr mgr_client;
	struct mg_mgr mgr_server;

	struct mg_connection* connection_server;

	void start_connections();

public:
	Server();

	bool settting_ok();

	void run();

	virtual ~Server();
};

#endif /* SERVER_H_ */
