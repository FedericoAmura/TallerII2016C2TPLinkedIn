/*
 * HttpParser.h
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_
#define APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_

#include "../fossa/fossa.h"
#include "../server/API_HttpRequest.h"
#include <string>

class HttpParser {
public:

	static struct ns_str* get_header(struct http_message* msg, const char* name);

	static int parse_header(struct ns_str *hdr, const char* var_name, char* buffer, size_t buf_size);

	static bool parse_username_password(struct http_message* msg, std::string &username, std::string &password);

	static bool parse_token(struct http_message* msg, std::string &token);
};

#endif /* APPSERVER_INCLUDE_PARSERS_HTTPPARSER_H_ */
