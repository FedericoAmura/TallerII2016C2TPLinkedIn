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
			std::cout << "ERROR >> Invalid uri " << std::endl;
			return http_response("", STATUS_BAD_REQUEST);
			break;
	}
	return res;
}

http_response PUT_Handler::handleProfile() {
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
