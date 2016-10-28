/*
 * req_res.h
 *
 *  Created on: Sep 5, 2016
 *      Author: emanuel
 */

#ifndef COMMON_REQ_RES_H_
#define COMMON_REQ_RES_H_

#include "../mongoose/mongoose.h"
#include "../common/Utils.h"

struct http_request{
	mg_connection* connection;
	http_message* message;

	http_request(mg_connection* c, http_message* msg){
		connection = c;
		message = msg;
	}

	METHOD method(){
		if (mg_vcmp(&message->method, GET) == 0)
			return _GET;
		if (mg_vcmp(&message->method, POST) == 0)
			return _POST;
		if (mg_vcmp(&message->method, DELETE) == 0)
			return _DELETE;
		if (mg_vcmp(&message->method, PUT) == 0)
			return _PUT;
		return _INVALID_METHOD;
	}

	std::string uri(){
		std::string _uri;
		_uri.assign(message->uri.p, message->uri.len);
		return _uri;
	}

	std::string query_string() {
		std::string qs;
		qs.assign(message->query_string.p, message->query_string.len);
		if (qs.empty())
			return "";
		return ("?" + qs);
	}
};

struct http_response {
	std::string message;
	int res_code;

	http_response() {
		message = "";
		res_code = 0;
	}

	http_response(std::string msg, int status) {
		message = msg;
		res_code = status;
	}
};
#endif /* COMMON_REQ_RES_H_ */
