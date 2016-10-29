/*
 * DELETE_Handler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_DELETEHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_DELETEHANDLER_H_

#include "HTTPRequestHandler.h"

class DELETE_Handler : public HTTPRequestHandler {
private:
	/*
	 * Cerrar sesión
	 * @return
	 * */
	http_response handle_logout();

	/*
	 * Rechazar solicitud de contacto
	 * @return
	 * */
	http_response handle_reject_contact_request();

	/*
	 * Eliminar contacto
	 * @return
	 * */
	http_response handle_delete_contact();

public:
	/*
	 * Constructor
	 * @param req	petición http que se desea manejar.
	 * */
	DELETE_Handler(http_request* req);

	/*
	 * Manejar Request
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			y código de respuesta correctamente, sino la información es
	 * 			vacía y el código de respuesta queda específicado en http_response.res_code
	 * */
	http_response handleRequest();

	/*
	 * Destructor
	 * */
	virtual ~DELETE_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_DELETEHANDLER_H_ */
