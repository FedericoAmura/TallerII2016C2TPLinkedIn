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
			std::cout << "ERROR >> Method Not Allowed" << std::endl;
			return http_response("", STATUS_MET_NOT_ALLOWED);
			break;
	}
	return res;
}

http_response POST_Handler::handleLogIn() {
	std::string username, password;
	bool parsed_username = HttpParser::parse_variable_from_authorization_header(request->message, USERNAME, username);
	bool parsed_password = HttpParser::parse_variable_from_authorization_header(request->message, PASSWORD, password);

	if (!parsed_username || !parsed_password) {
		std::cout << "Error: username and/or password not found to parse. LogIn failed" << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data = Json::object{ {"username", username},
							  {"password", password} };
	uint32_t user_id;
	try {
		user_id = db_json->login(data);
	} catch (NonexistentUsername &e) {
		std::cout << "Error: Non Existent Username. LogIn failed" << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	} catch (BadPassword &e) {
		std::cout << "Error: User unauthorized. Bad Password. LogIn failed" << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	std::cout << "Info: LogIn OK" << std::endl;
	std::string userID = std::to_string(user_id);
	Json resp = Json::object { {"userID", userID} };
	return http_response(resp.dump(), STATUS_OK);
}

http_response POST_Handler::handleSignUp() {
	std::string json_string(request->message->body.p);
	std::string err;
	Json data = Json::parse(json_string, err);

	if (!err.empty()){
		std::cout << "Error: Invalid Json Format. Sign Up failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	uint32_t user_id;
	try {
		user_id = db_json->registrarse(data);
	} catch (PreexistentUsername &e) {
		std::cout << "Error: PreExistent Username. Sing Up failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	} catch (NonexistentEmail &e) {
		std::cout << "Error: Non Existent Email. Sing Up failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	} catch (BadGeolocation &e) {
		std::cout << "Error: Bad Geolocation. Sing Up failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	} catch (MalformedDate &e) {
		std::cout << "Error: Malformed Date. Sing Up failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	std::cout << "Info: SignUp OK" << std::endl;
	std::string userID = std::to_string(user_id);
	Json resp = Json::object { {"userID", userID} };
	return http_response(resp.dump(), STATUS_CREATED);
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
