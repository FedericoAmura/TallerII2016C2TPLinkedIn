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

	std::string body(request->message->body.p), err;
	Json data = Json::parse(body, err);
	if ( !err.empty() ) {
		std::cout << "Error: Invalid Json Format. Update Profile failed." << std::endl;
		return http_response("", STATUS_UNPROCCESABLE);
	}

	uint32_t userID = std::stoi(userID_s);
	try {
		db_json->setDatos(userID, data);
	} catch (NonexistentUserID &e) {

	} catch (NonexistentSkill &e) {

	} catch (NonexistentPosition &e) {

	}

	return http_response("{\"msg\":\"Edit my profile\"}\n", STATUS_OK);
}

http_response PUT_Handler::handleSummary() {
	return http_response("{\"msg\":\"Edit my summary\"}\n", STATUS_OK);
}

http_response PUT_Handler::handlePhoto() {
	return http_response("{\"msg\":\"Edit my photo\"}\n", STATUS_OK);
}

http_response PUT_Handler::handleRecommend() {
	return http_response("{\"msg\":\"Recommend\"}\n", STATUS_OK);
}

PUT_Handler::~PUT_Handler() {
}
