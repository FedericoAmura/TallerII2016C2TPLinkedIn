/*
 * HttpParser.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_
#define APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_

#include "../mongoose/mongoose.h"
#include "../server/API_HttpRequest.h"
#include "../common/Exceptions.h"
#include "../common/Utils.h"
#include <stdlib.h>

using json11::Json;

class HttpParser {
private:
	static struct mg_str* get_header(struct http_message* msg, const char* name);

	static int parse_header(struct mg_str *hdr, const char* var_name, char* buffer, size_t buf_size);

public:

	static bool parse_variable_from_authorization_header(struct http_message* msg, const std::string var_name, std::string &buffer);

	static Json parse_user_search(struct http_message* msg);

	static Json parse_json_from_body(struct http_message* msg);

	static std::string parse_variable_from_query_string(struct http_message* msg, const std::string var_name);
};

#endif /* APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_ */
