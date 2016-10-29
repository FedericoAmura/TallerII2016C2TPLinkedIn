/*
 * POST_Handler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_POSTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_POSTHANDLER_H_

#include "HTTPRequestHandler.h"

class POST_Handler : public HTTPRequestHandler {
private:
	/*
	 * Iniciar Sesión
	 * @return	en caso de que los datos administrados sean correctos, se devolverá
	 * 			un token de sesión, sino se devolverá un código de error en el http_response
	 * */
	http_response handle_login();

	/*
	 * Registrarse
	 * @return	en caso de que los datos administrados sean correctos, se devolverá
	 * 			un token de sesión, sino se devolverá un código de error en el http_response
	 * */
	http_response handle_signup();

	/*
	 * Aceptar una solicitud de contacto
	 * @return
	 * */
	http_response handle_accept_contact_request();

	/*
	 * Solicitud para agregar un contacto
	 * @return
	 * */
	http_response handle_create_contact_request();

	/*
	 * Notificar que el mensaje fue visto
	 * @return
	 * */
	http_response handle_notify_message_seen();

	/*
	 * Enviar un mensaje a un chat
	 * @return
	 * */
	http_response handle_send_message();

public:
	/*
	 * Constructor
	 * @param req	petición http que se desea manejar.
	 * */
	POST_Handler(http_request* req);

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
	virtual ~POST_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_POSTHANDLER_H_ */
