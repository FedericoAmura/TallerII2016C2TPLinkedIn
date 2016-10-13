/*
 * JsonParser.h
 *
 *  Created on: Oct 6, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_PARSERS_JSONPARSER_H_
#define APPSERVER_INCLUDE_HANDLERS_PARSERS_JSONPARSER_H_

#include "../json11/json11.hpp"
#include "../common/User.h"
#include "../server/API_HttpRequest.h"
#include <stdlib.h>

class JsonParser {

public:
	static json11::Json parseStringToJson(std::string data);

	static bool parse_user_data(struct user_record &rec, json11::Json data);

	static bool parse_user_update(struct user_update &rec, json11::Json data);

	static bool parse_parameter(std::string &buffer, const std::string param, json11::Json data);

	static bool parse_contact_request(struct contact_request &cont_req, json11::Json data);

	static bool parse_recommend_request(struct recommend_request &recommend_req, json11::Json data);

};

#endif /* APPSEVER_INCLUDE_HANDLERS_PARSERS_JSONPARSER_H_ */
