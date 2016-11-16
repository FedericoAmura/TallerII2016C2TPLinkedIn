//============================================================================
// Name        : AppServer.cpp
// Author      : Taller2
// Version     :
// Copyright   : Do not copy
// Description : Jobify: Aplicación estilo linkedin, servidor de aplicación
//============================================================================

#include "../include/server/Server.h"

int main(int argc, char** argv) {
	std::cout << "Application Server" << std::endl;

	if (argc > 2) {
		std::cout << "Invalid amount of arguments. Run \"--help\" command for help." << std::endl;
		return -1;
	}

	bool gcm_mode = true;
	if (argc == 2) {
		if (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0) {
			std::cout << "Commands:" << std::endl;
			std::cout << "\t-g --gcm\tRuns the application using the Google Cloud Messaging service." << std::endl;
			std::cout << "\t-t --test\tRuns the application in test mode." << std::endl;
			std::cout << "\n\tIf you do not enter arguments, the application will run using the Google Cloud Messaging service by default." << std::endl;
			return 0;
		} else if (strcmp(argv[1],"-t") == 0 || strcmp(argv[1],"--test") == 0) {
			std::cout << "Running Application Server in test mode." << std::endl;
			gcm_mode = false;
		} else if (strcmp(argv[1],"-g") == 0 || strcmp(argv[1],"--gcm") == 0) {
			std::cout << "Running Application Server using the Google Cloud Messaging service." << std::endl;
		}  else {
			std::cout << "Invalid command. Run \"--help\" command for help." << std::endl;
			return -1;
		}
	} else
		std::cout << "Running Application Server using the Google Cloud Messaging service." << std::endl;

	Server* server = new Server(gcm_mode);
	if (server->setting_ok())
		server->run();
	delete server;
	return 0;
}
