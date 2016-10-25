/*
 * PUT_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/PUT_Handler.h"
#include <iostream>

PUT_Handler::PUT_Handler(http_request* req) : HTTPRequestHandler(req) {
}

http_response PUT_Handler::handleRequest() {
	http_response res;
	switch (uri){
		case _USER_PROFILE:
			// /users/<user_id>
			res = handleProfile();
			break;
		case _USER_SUMMARY:
			// /users/<user_id>/summary
			res = handleSummary();
			break;
		case _USER_PHOTO:
			// /users/<user_id>/photo
			res = handlePhoto();
			break;
		case _POPULAR_RECOMMEND:
			// /users/popular/<user_id1>/<user_id2>
			res = handleRecommend();
			break;
		default:
			std::cout << "ERROR >> Method Not Allowed" << std::endl;
			return http_response("", STATUS_MET_NOT_ALLOWED);
			break;
	}
	return res;
}

http_response PUT_Handler::handleProfile() {
	/* uri = /users/<userID> */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID_s = vec_uri[1];
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Update profile failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

/*	TODO
	if (validate_token) {
		std::cout << "Error: Invalid token and userID. User unauthorized. Update profile failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}
*/
	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		std::cout << "Error: Invalid Json Format. Update Profile failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	uint32_t userID = std::stoi(userID_s);
	try {
		db_json->setDatos(userID, data);
	} catch (NonexistentUserID &e) {
		std::cout << "Error: Non existent userID" << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	} catch (NonexistentSkill &e) {
		std::cout << "Error: Non existent skill" << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	} catch (NonexistentPosition &e) {
		std::cout << "Non existent position" << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	return http_response("", STATUS_OK);
}

http_response PUT_Handler::handleSummary() {
	/* uri = /users/<userID>/summary */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID_s = vec_uri[1];
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Update profile failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

/*	TODO
	if (validate_token) {
		std::cout << "Error: Invalid token and userID. User unauthorized. Update profile failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}
*/
	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		std::cout << "Error: Invalid Json Format. Update Summary failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	uint32_t userID = std::stoi(userID_s);
	try {
		db_json->setResumen(userID, data);
	} catch (NonexistentUserID &e) {
		std::cout << "Error: UserID not found. Update summary failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}
	return http_response("", STATUS_NO_CONTENT);
}

http_response PUT_Handler::handlePhoto() {
	/* uri = /users/<userID>/photo */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID_s = vec_uri[1];
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Update Photo failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

/*	TODO
	if (validate_token) {
		std::cout << "Error: Invalid token and userID. User unauthorized. Update profile failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}
*/
	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		std::cout << "Error: Invalid Json Format. Update Photo failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	uint32_t userID = std::stoi(userID_s);
	try {
		db_json->setFoto(userID, data);
	} catch (NonexistentUserID &e) {
		std::cout << "Error: UserID not found. Update photo failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	} catch (TooBigException &e) {
		std::cout << "Error: Photo too big. Update photo failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}
	return http_response("", STATUS_NO_CONTENT);
}

http_response PUT_Handler::handleRecommend() {
	/* uri = /users/popular/<userID1>/<userID2> */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID1_s = vec_uri[2];
	std::string userID2_s = vec_uri[3];

	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Recommend user failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

/*	TODO
	if (validate_token) {
		std::cout << "Error: Invalid token and userID. User unauthorized. Update profile failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}
*/

	/* TODO validar que userID1 == recommender ??? */
	Json data;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		std::cout << "Error: Invalid Json Format. Recommend user failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	try {
		db_json->actualizarRecomendacion(data);
	} catch (NonexistentUserID &e) {
		std::cout << "Error: Invalid Json Format. Recommend user failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}
	return http_response("", STATUS_NO_CONTENT);
}

PUT_Handler::~PUT_Handler() {
}
