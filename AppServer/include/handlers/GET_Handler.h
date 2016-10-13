/*
 * GET_Handler.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_GET_HANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_GET_HANDLER_H_

#include "HTTPRequestHandler.h"

class GET_Handler : public HTTPRequestHandler {
private:
	/*
	 * Busqueda de usuarios
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre los usuarios que cumplan con los parametros específicados
	 * 			en la uri y el código de respuesta, sino devuelve una respuesta
	 * 			vacía y el código de respuesta queda específicado en http_response.res_code
	 * */
	http_response handleUsers();

	/*
	 * Perfil de un usuario
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre el perfil del usuario solicitado y el código de respuesta,
	 * 			sino devuelve una respuesta vacía y el código de respuesta
	 * 			queda específicado en http_response.res_code
	 * */
	http_response handleUserProfile();

	/*
	 * Résumen de un usuario
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			resumida sobre sobre el usuario solicitado y el código de respuesta,
	 * 			sino devuelve una respuesta vacía y el código de respuesta
	 * 			queda específicado en http_response.res_code
	 * */
	http_response handleUserSummary();

	/*
	 * Foto de un usuario
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre la foto de un usuario solicitado y el código de respuesta,
	 * 			sino devuelve una respuesta vacía y el código de respuesta queda
	 * 			específicado en http_response.res_code
	 * */
	http_response handleUserPhoto();

	/*
	 * Thumbnail de la foto
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre el thumbnail de la foto de un usuario y el código de respuesta,
	 * 			sino devuelve una respuesta	vacía y el código de respuesta
	 * 			queda específicado en http_response.res_code
	 * */
	http_response handleUserThumb();

	/*
	 * Bief de un usuario
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre el nombre/popularidad/ciudad/thumbnail_foto del usuario
	 * 			y código de respuesta, sino devuelve una respuesta	vacía y
	 * 			el código de respuesta queda específicado en http_response.res_code
	 * */
	http_response handleUserBrief();

	/*
	 * Notificaciones de un usuario
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre las notificaciones de contacto pendiente (sólo las recibidas, sólo
	 * 			si está autenticado) y código de respuesta, sino devuelve una respuesta	vacía y
	 * 			el código de respuesta queda específicado en http_response.res_code
	 * */
	http_response handleUsersNotif();

	/*
	 * Notificaciones pendiente de un usuario
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre el #  de notificaciones de contacto pendiente y
	 * 			código de respuesta, sino devuelve una respuesta vacía y
	 * 			el código de respuesta queda específicado en http_response.res_code
	 * */
	http_response handleUsersNewNotif();

	/*
	 * Notificación sobre solicitud de contacto
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre la solicitud (si es que existe) y el
	 * 			código de respuesta, sino devuelve una respuesta vacía y
	 * 			el código de respuesta queda específicado en http_response.res_code
	 * */
	http_response handleUsersNotifyRequest();

	/*
	 * Contactos de un usuario
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			sobre los contactos de un usuario y el código de respuesta,
	 * 			sino devuelve una respuesta vacía y	el código de respuesta
	 * 			queda específicado en http_response.res_code
	 * */
	http_response handleUsersContacts();

	/*
	 * Consulta sobre conexión con otro usuario
	 * @return	en caso de éxito, retorna un http_response con información
	 * 			si los usuarios están conectados o no y el código de respuesta,
	 * 			sino devuelve una respuesta vacía y	el código de respuesta
	 * 			queda específicado en http_response.res_code
	 * */
	http_response handleUsersContactManagement();

	/*
	 * Usuarios más populares
	 * @return	en caso de éxito, retorna los 10 usuarios más populares
	 * 			sino devuelve "" y el código de respuesta queda específicado
	 * 			en http_response.res_code
	 * */
	http_response handlePopular();

	/*
	 * Recomendación entre usuarios
	 * @return	en caso de éxito, retorna información de si un usuario recomendó
	 * 			a otro, sino devuelve "" y el código de respuesta queda específicado
	 * 			en http_response.res_code
	 * */
	http_response handlePopularRecommend();

	/*
	 * Populares por posición
	 * @return	en caso de éxito, retorna información sobre los usuarios más populares
	 * 			por la posición específicada y el código de respuesta queda específicado
	 * 			en http_response.res_code, sino retorna información nula.
	 * */
	http_response handlePopularByPosition();

	/*
	 * Populares por skill
	 * @return	en caso de éxito, retorna información sobre los usuarios más populares
	 * 			por la skill específicada y el código de respuesta queda específicado
	 * 			en http_response.res_code, sino retorna información nula.
	 * */
	http_response handlePopularBySkill();

	/*
	 * Notificaciones del Chat
	 * @return	en caso de éxito, retorna información sobre mensajes sin leer del usuario
	 * 			(si está autenticado) y el código de respuesta queda específicado
	 * 			en http_response.res_code, sino retorna información nula.
	 * */
	http_response handleChatNew();

	/*
	 * Ultimo mensaje de la conversación
	 * @return	en caso de éxito, retorna información sobre el # del último
	 * 			mensaje en la conversación (si está autenticado y son contactos)
	 * 			y el código de respuesta queda específicado	en http_response.res_code,
	 * 			sino retorna información nula.
	 * */
	http_response handleChatLastMsg();

	/*
	 * Mensajes comprendidos entre un período
	 * @return	en caso de éxito, retorna los mensajes comprendidos entre los parametros
	 * 			recibidos (si el usuario está autenticado y son contactos)
	 * 			y el código de respuesta queda específicado	en http_response.res_code,
	 * 			sino retorna información nula.
	 * */
	http_response handleChatIncMsg();

	/*
	 * Categorías
	 * @return	en caso de éxito, retorna todas las categorías disponibles.
	 * */
	http_response handleCategories();

	/*
	 * Posición de trabajo por categoría
	 * @return	en caso de éxito, retorna las posiciones de trabajo asociadas a una
	 *			categoría.
	 * */
	http_response handleJobPositionsByCategory();

	/*
	 * Posición de trabajo particular
	 * @return	en caso de éxito, retorna información sobre una posición de trabajo
	 *			en particular.
	 * */
	http_response handleJobPosition();

	/*
	 * Skills por categoría
	 * @return	en caso de éxito, retorna los skills asociadas a una categoría.
	 * */
	http_response handleSkillsByCategory();

	/*
	 * Skill particular
	 * @return	en caso de éxito, retorna información sobre una skill en particular.
	 * */
	http_response handleSkill();

public:
	/*
	 * Constructor
	 * @param req	petición http que se desea manejar.
	 * */
	GET_Handler(http_request* request);

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
	virtual ~GET_Handler();
};

#endif /* APPSERVER_INCLUDE_HANDLERS_GETHANDLER_H_ */
