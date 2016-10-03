/*
 * DELETE_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/DELETE_Handler.h"

DELETE_Handler::DELETE_Handler(http_request* req) : HTTPRequestHandler(req) {
}

http_response DELETE_Handler::handleRequest() {
	http_response res;
	switch (uri){
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
			return http_response("", STATUS_BAD_REQUEST);
			break;
	}
	return res;
}

http_response DELETE_Handler::handleJobPositions() {
	return http_response("", STATUS_NO_CONTENT);
}

http_response DELETE_Handler::handleSkills() {
	return http_response("", STATUS_NO_CONTENT);
}

http_response DELETE_Handler::handleCategories() {
	return http_response("", STATUS_NO_CONTENT);
}

DELETE_Handler::~DELETE_Handler() {
}

