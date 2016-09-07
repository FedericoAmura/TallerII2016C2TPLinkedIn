/*
 * RequestHandler.h
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#ifndef HANDLERS_REQUESTHANDLER_H_
#define HANDLERS_REQUESTHANDLER_H_

#include "../common/req_res.h"

class RequestHandler{
private:
	http_request* request;

public:
	RequestHandler(http_request* req);

	void process();

	virtual ~RequestHandler();
};

#endif /* HANDLERS_REQUESTHANDLER_H_ */
