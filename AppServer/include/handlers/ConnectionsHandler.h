/*
 * ConnectionsHandler.h
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_

#include "../../include/handlers/RequestHandler.h"
#include <signal.h>

static bool running = false;

static void SIGINT_Handler(int signum) {
    running = false;
};

class ConnectionsHandler {
 private:
	struct mg_mgr mgr;
	struct mg_connection* connection;


 public:
	ConnectionsHandler(DBJSON* db_json);

    /**
     * Verificación de inicialización de recursos
     * @return  true si todos los recursos fueron inicializados correctamente
     *          sino false
     */
	bool initialized();

    /**
     * Escuchar conexiones
     */
	void run();

	virtual ~ConnectionsHandler();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_CONNECTIONSHANDLER_H_
