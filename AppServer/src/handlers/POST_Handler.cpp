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
			res = handle_login();
			break;
		case _SIGNUP:
			// /signup
			res = handle_signup();
			break;
		case _USERS_REQ_CONTACT:
			// /users/<user_id>/notif/<user_id_contact>
			res = handle_accept_contact_request();
			break;
		case _USERS_CONTACTS:
			// /users/<user_id>/contacts
			res = handle_create_contact_request();
			break;
		case _CHAT_NEW:
			// /chat/<user_id>/new
			res = handle_notify_message_seen();
			break;
		case _CHAT_CHATS:
			// /chat/<user_id1>/<user_id2>
			res = handle_send_message();
			break;
		default:
			std::cout << "ERROR >> Method Not Allowed" << std::endl;
			return http_response("", STATUS_MET_NOT_ALLOWED);
			break;
	}
	return res;
}

http_response POST_Handler::handle_login() {
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
	std::string token = "";
	try {
		user_id = db_json->login(data);
		token = db_json->generarToken(data);
	} catch (NonexistentUsername &e) {
		std::cout << "Error: Non Existent Username. LogIn failed" << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	} catch (BadPassword &e) {
		std::cout << "Error: User unauthorized. Bad Password. LogIn failed" << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	std::cout << "Info: LogIn OK " << std::endl;
	std::string userID = std::to_string(user_id);
	Json resp = Json::object { {"userID", userID},
 							   {"token" , token} };
	return http_response(resp.dump(), STATUS_OK);
}

http_response POST_Handler::handle_signup() {
	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
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
	Json resp = Json::object { {"userID", userID} }; /* TODO es necesario??? */
	return http_response(resp.dump(), STATUS_CREATED);
}

http_response POST_Handler::handle_accept_contact_request() {
	/* uri = /users/<userID>/contacts/<other_userID> */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID1_s = vec_uri[1];
	std::string userID2_s = vec_uri[3];
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Accept contact request failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "Error: Invalid token. Non existent token. Accept contact request failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}catch (TokenHasExpired &e) {
		std::cout << "Error: Invalid token. Token has expired. Accept contact request failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		std::cout << "Error: Invalid Json Format. Accept contact request failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	uint32_t userID1 = std::stoi(userID1_s);
	uint32_t userID2 = std::stoi(userID2_s);
	try {
		db_json->aceptarPeticion(userID1, userID2);
	} catch (NonexistentRequest &e) {
		std::cout << "Error: Non existent request. Accept contact request failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}
	return http_response("", STATUS_NO_CONTENT);
}

http_response POST_Handler::handle_create_contact_request() {
	/* uri = /users/<userID>/contacts */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID_s = vec_uri[1];
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Create contact request failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "Error: Invalid token. Non existent token. Create contact request failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}catch (TokenHasExpired &e) {
		std::cout << "Error: Invalid token. Token has expired. Create contact request failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		std::cout << "Error: Invalid Json Format. Create contact request failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	try {
		db_json->crearPeticion(data);
	} catch (NonexistentUserID &e) {
		std::cout << "Error: Non existent userID. Create contact request failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}
	return http_response("", STATUS_CREATED);
}

http_response POST_Handler::handle_notify_message_seen() {
	/* uri = /chat/<userID>/new */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID_s = vec_uri[1];
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Notify Message Seen failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "Error: Invalid token. Non existent token. Notify Message Seen failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}catch (TokenHasExpired &e) {
		std::cout << "Error: Invalid token. Token has expired. Notify Message Seen failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		std::cout << "Error: Invalid Json Format. Notify Message Seen failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	try {
		db_json->marcarChatLeido(data);
	} catch (NonexistentChat &e) {
		std::cout << "Error: Non existent chat. Notify Message Seen failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}
	return http_response("", STATUS_NO_CONTENT);
}

http_response POST_Handler::handle_send_message() {
	/* uri = /chat/<userID1>/<userID2> */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID_s = vec_uri[1];
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Send Message failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "Error: Invalid token. Non existent token. Send Message failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}catch (TokenHasExpired &e) {
		std::cout << "Error: Invalid token. Token has expired. Send Message failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		std::cout << "Error: Invalid Json Format. Send Message failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	try {
		db_json->enviarMensaje(data);
	} catch (NonexistentUserID &e) {
		std::cout << "Error: Non existent userID. Send Message failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}
	return http_response("", STATUS_CREATED);
}

POST_Handler::~POST_Handler() {
}
