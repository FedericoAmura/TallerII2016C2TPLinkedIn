/*
 * SharedServerHandler.cpp
 *
 *  Created on: Sep 3, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/SharedServerHandler.h"

SharedServerHandler::SharedServerHandler(http_request* req) {
	request = req;
}

SharedServerHandler::~SharedServerHandler() {
}

