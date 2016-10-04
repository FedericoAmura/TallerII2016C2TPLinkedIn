/*
 * PUT_Handler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_PUTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_PUTHANDLER_H_

#include "HTTPRequestHandler.h"

class PUT_Handler : public HTTPRequestHandler {
private:
	/*
	 * Actualización de perfil
	 * @return
	 * */
	http_response handleProfile();

	/*
	 * Actualización de Resumen de usuario
	 * @return
	 * */
	http_response handleSummary();

	/*
	 * Actualización de Foto
	 * @return
	 * */
	http_response handlePhoto();

	/*
	 * Recomendar usuario
	 * @return
	 * */
	http_response handleRecommend();

public:
	/*
	 * Constructor
	 * @param req	petición http que se desea manejar.
	 * */
	PUT_Handler(http_request* req);

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
	virtual ~PUT_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_PUTHANDLER_H_ */
