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
			res = handleLogIn();
			break;
		case _SIGNUP:
			res = handleSignUp();
			break;
		case _POPULAR_RECOMMEND:
			res = handleRecommendations();
			break;
		case _USERS_CONTACTS:
			res = handleAddContact();
			break;
		case _CHAT_NEW:
			res = handleChat();
			break;
		case _JOB_POSITIONS:
			res = handleJobPositions();
			break;
		case _SKILLS:
			res = handleSkills();
			break;
		case _CATEGORIES:
			res = handleCategories();
			break;
		default:
			res = http_response("", STATUS_BAD_REQUEST);
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
	 * y generar un token de sesión (en caso de que todo salga bien, enviar el token)
	 * */
	bool valid_user = db_handler->validateUserPass(username, password);
	if (!valid_user) {
		std::cout << "LogIn failed" << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	std::string token = db_handler->generateToken(username, password);
	std::string msg = "{\"token\":\"" + token + "\"}";
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
		return http_response("", STATUS_BAD_REQUEST);
	}

	/* TODO generar un token desde la base de datos con el username y la password */
	std::string user = _json["register"]["email"].string_value();
	std::string pass = _json["register"]["pass"].string_value();

	std::string token = db_handler->generateToken(user,pass);
	std::string msg = "{\"token\":\"" + token + "\"}";
	return http_response(msg, STATUS_CREATED);
}

http_response POST_Handler::handleRecommendations() {
	std::cout << request->message->body.p << std::endl;
	return http_response("{\"msg\":\"Recommendations\"}", STATUS_CREATED);
}

http_response POST_Handler::handleAddContact() {
	return http_response("{\"msg\":\"AddContact\"}", STATUS_CREATED);
}

http_response POST_Handler::handleChat() {
	return http_response("{\"msg\":\"Chat\"}", STATUS_CREATED);
}

http_response POST_Handler::handleJobPositions() {
	return http_response("{\"msg\":\"JobPositions\"}", STATUS_CREATED);
}

http_response POST_Handler::handleSkills() {
	return http_response("{\"msg\":\"Skills\"}", STATUS_CREATED);
}

http_response POST_Handler::handleCategories() {
	return http_response("{\"msg\":\"Categories\"}", STATUS_CREATED);
}

POST_Handler::~POST_Handler() {
}

