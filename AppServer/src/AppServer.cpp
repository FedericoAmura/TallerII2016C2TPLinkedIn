//============================================================================
// Name        : AppServer.cpp
// Author      : Taller2
// Version     :
// Copyright   : Do not copy
// Description : Jobify: Aplicación estilo linkedin, servidor de aplicación
//============================================================================

#include "../include/server/Server.h"

int main() {
	Server* server = new Server();
	if (server->setting_ok())
		server->run();
	delete server;
	return 0;
}
