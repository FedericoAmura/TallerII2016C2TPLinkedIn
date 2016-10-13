/*
 * HTTPRequestHandler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_HTTPREQUESTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_HTTPREQUESTHANDLER_H_

#include "../common/req_res.h"
#include "../parsers/HttpParser.h"
#include "../parsers/JsonParser.h"
//#include "../cpr/cpr.h"
#include "DB_Handler.h"
#include "../common/Utils.h"

class HTTPRequestHandler {
protected:
	http_request* request;
	DB_Handler* db_handler;
	URI uri;

	//bool validToken();

public:
	/*
	 * Constructor
	 * @param req	petición http que se desea manejar.
	 * */
	HTTPRequestHandler(http_request* req);

	/*
	 * Manejar Request
	 * Maneja la petición http retornando
	 * una respuesta que contiene el mensaje y
	 * el código de la operación.
	 * */
	virtual http_response handleRequest() = 0;

	/*
	 * Enviar Respuesta
	 * Envía una respuesta http a aquel que realizó
	 * la petición.
	 * @param res	respuesta http que se enviará.
	 * */
	void sendReply(http_response* res);


	/*
	 * Cerrar Conexión
	 * Cierra la conexión establecida
	 * */
	void closeConnection();

	/*
	 * Destructor
	 * */
	virtual ~HTTPRequestHandler(){};
};



#endif /* APPSERVER_INCLUDE_HANDLERS_HTTPREQUESTHANDLER_H_ */
