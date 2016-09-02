//============================================================================
// Name        : AppServer.cpp
// Author      : Taller2
// Version     :
// Copyright   : Do not copy
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "server/Server.h"

int main(){
	Server* server = new Server();
	if (server->settting_ok())
		server->run();
	delete server;
	return 0;
}
