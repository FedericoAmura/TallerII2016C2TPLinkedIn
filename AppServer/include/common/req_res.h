/*
 * req_res.h
 *
 *  Created on: Sep 5, 2016
 *      Author: emanuel
 */

#ifndef COMMON_REQ_RES_H_
#define COMMON_REQ_RES_H_

#include "../fossa/fossa.h"

struct http_request{
	ns_connection* connection;
	http_message* message;

	http_request(ns_connection* c, http_message* msg){
		connection = c;
		message = msg;
	}
};

#endif /* COMMON_REQ_RES_H_ */
