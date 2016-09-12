/*
 * req_res.h
 *
 *  Created on: Sep 5, 2016
 *      Author: emanuel
 */

#ifndef COMMON_REQ_RES_H_
#define COMMON_REQ_RES_H_

#include "../fossa/fossa.h"
#include "../server/API_Server.h"

struct http_request{
	ns_connection* connection;
	http_message* message;

	http_request(ns_connection* c, http_message* msg){
		connection = c;
		message = msg;
	}

	std::string method(){
		if (ns_vcmp(&message->method, GET) == 0)
			return GET;
		if (ns_vcmp(&message->method, POST) == 0)
			return POST;
		if (ns_vcmp(&message->method, DELETE) == 0)
			return DELETE;
		if (ns_vcmp(&message->method, PUT) == 0)
			return PUT;
		return PUT;		//hardcode
	}

	std::string uri(){
		std::string _uri;
		_uri.assign(message->uri.p, message->uri.len);
		return _uri;
	}
};


struct http_response{
	ns_connection* connection;
	http_message* response;

	http_response(ns_connection* c, http_message* res){
		connection = c;
		response = res;
	}

	int status(){
		return response->resp_code;
	}

	const char* body(){
		return response->body.p;
	}

	int contentLength(){
		return response->body.len;
	}


};
#endif /* COMMON_REQ_RES_H_ */
