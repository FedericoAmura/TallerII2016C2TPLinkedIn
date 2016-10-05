/*
 * POST_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/POST_Handler.h"
#include <iostream>

POST_Handler::POST_Handler(http_request* req) : HTTPRequestHandler(req) {
}

http_response POST_Handler::handleRequest() {
	http_response res;
	switch (uri){
		case _LOGIN:
			// /login
			res = handleLogIn();
			break;
		case _SIGNUP:
			// /signup
			res = handleSignUp();
			break;
		case _USERS_REQ_CONTACT:
			// /users/<user_id>/notif/<user_id_contact>
			res = handleAcceptContactRequest();
			break;
		case _USERS_CONTACTS:
			// /users/<user_id>/contacts
			res = handleContactRequest();
			break;
		case _CHAT_NEW:
			// /chat/<user_id>/new
			res = handleChatNotifyMsgSeen();
			break;
		case _CHAT_CHATS:
			// /chat/<user_id1>/<user_id2>
			res = handleChatSendMsg();
			break;
		default:
			std::cout << "ERROR >> Invalid uri " << std::endl;
			res = http_response("", STATUS_NOT_FOUND);
			break;
	}
	return res;
}

http_response POST_Handler::handleLogIn() {
	std::string username, password;
	bool parsed = HttpParser::parse_username_password(request->message, username, password);

	if (!parsed) {
		std::cout << "LogIn failed" << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	/* TODO validar el username y password en la base de datos
	 * y generar un token de sesión y un user_id (en caso de que todo
	 * salga bien, enviar el token y el user_id)
	 * */
	bool valid_user = db_handler->validateUserPass(username, password);
	if (!valid_user) {
		std::cout << "LogIn failed" << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	std::string token = db_handler->generateToken(username, password);
	std::string msg = "{\"userID\":\"" + username + "\", \"token\":\"" + token + "\"}\n";
	return http_response(msg, STATUS_OK);
}

http_response POST_Handler::handleSignUp() {
	std::string json_string(request->message->body.p);
	std::string err;
	json11::Json _json = json11::Json::parse(json_string, err);

	if (!err.empty()){
		std::cout << "SignUp failed" << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}

	try {
		db_handler->registerNewUser(_json);
	} catch (InvalidJsonException &e) {
		std::cout << "Invalid Json Format" << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	return http_response("", STATUS_CREATED);
}

http_response POST_Handler::handleAcceptContactRequest() {
	return http_response("{\"msg\":\"Contact request accepted\"}\n", STATUS_CREATED);
}

http_response POST_Handler::handleContactRequest() {
	return http_response("{\"msg\":\"Contact Request\"}\n", STATUS_CREATED);
}

http_response POST_Handler::handleChatNotifyMsgSeen() {
	return http_response("{\"msg\":\"Message seen\"}\n", STATUS_CREATED);
}

http_response POST_Handler::handleChatSendMsg() {
	return http_response("{\"msg\":\"Send message\"}\n", STATUS_CREATED);
}

POST_Handler::~POST_Handler() {
}

