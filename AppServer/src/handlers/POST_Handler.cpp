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
			std::cout << "Error: Invalid uri " << std::endl;
			res = http_response("", STATUS_NOT_FOUND);
			break;
	}
	return res;
}

http_response POST_Handler::handleLogIn() {
	std::string username, password;
	bool parsed_username = HttpParser::parse_variable_from_authorization_header(request->message, USERNAME, username);
	bool parsed_password = HttpParser::parse_variable_from_authorization_header(request->message, PASSWORD, password);
	//bool parsed = HttpParser::parse_username_password(request->message, username, password);

	if (!parsed_username || !parsed_password) {
		std::cout << "Error: username and/or password not found to parse. LogIn failed" << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	/* TODO validar el username y password en la base de datos
	 * y generar un token de sesión y un user_id (en caso de que todo
	 * salga bien, enviar el token y el user_id)
	 * */
	bool valid_user = db_handler->validateUserPass(username, password);
	if (!valid_user) {
		std::cout << "Error: User unauthorized. LogIn failed" << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	json11::Json data = db_handler->generateJsonWithTokenAndUserID(username, password);
	std::cout << "Info: LogIn OK" << std::endl;
	return http_response(data.dump(), STATUS_OK);
}

http_response POST_Handler::handleSignUp() {
	std::string json_string(request->message->body.p);
	std::cout << json_string << std::endl;
	std::string err;
	json11::Json _json = json11::Json::parse(json_string, err);

	if (!err.empty()){
		std::cout << "Error: Invalid Json Format. Sign Up failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	user_record new_user;
	bool parsed = JsonParser::parse_user_data(new_user, _json);

	if (!parsed) {
		std::cout << "Error: Missing Data to register. Sign Up failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	try {
		db_handler->registerNewUser(new_user);
	} catch (ExistingUserException &e) {
		std::cout << "Error: Existing User. Sing Up failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}

	std::cout << "Info: SignUp OK" << std::endl;
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
