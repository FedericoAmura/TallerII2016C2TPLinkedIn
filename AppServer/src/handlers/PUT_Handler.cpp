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
			std::cout << "Error: Invalid uri. " << std::endl;
			return http_response("", STATUS_BAD_REQUEST);
			break;
	}
	return res;
}

http_response PUT_Handler::handleProfile() {
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string userID = vec_uri[1];
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "Error: Token not found. User unauthorized. Update profile failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	if (!db_handler->validateTokenAndUserID(token, userID)) {
		std::cout << "Error: Invalid token and userID. User unauthorized. Update profile failed." << std::endl;
		return http_response("", STATUS_UNAUTHORIZED);
	}

	std::string json_string(request->message->body.p);
	json11::Json _json = JsonParser::parseStringToJson(json_string);
	if (_json.is_null()) {

	}
	user_update user;
	parsed = JsonParser::parse_user_update(user, _json);


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
