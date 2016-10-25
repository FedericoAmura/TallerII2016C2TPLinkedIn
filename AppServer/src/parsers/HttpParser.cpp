/*
 * HttpParser.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/parsers/HttpParser.h"

struct mg_str* HttpParser::get_header(struct http_message* msg, const char* name) {
	return mg_get_http_header(msg, name);
}

int HttpParser::parse_header(struct mg_str *hdr, const char* var_name, char* buffer, size_t buf_size) {
	return mg_http_parse_header(hdr, var_name, buffer, buf_size);
}

bool HttpParser::parse_variable_from_authorization_header(struct http_message* msg, const std::string var_name, std::string &buffer) {
	struct mg_str* hdr = HttpParser::get_header(msg, AUTHORIZATION);
	if (!hdr)
		return false;
	int BUFFER_SIZE = 100;
	char buf[BUFFER_SIZE];

	int parsed_var = HttpParser::parse_header(hdr, var_name.c_str(), buf, BUFFER_SIZE);
	if (!parsed_var)
		return false;

	buffer = std::string(buf);
	return true;
}

bool HttpParser::parse_user_properties(struct http_message* msg, struct user_properties &prop) {
	bool parsed = false;
	int BUFFER_SIZE = 100;
	char buffer[BUFFER_SIZE];
	int found;

	found = mg_get_http_var(&msg->query_string, CATEGORY, buffer, BUFFER_SIZE);
	if (found > 0) {
		prop.category = std::string(buffer);
		parsed = true;
	}

	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, SKILL, buffer, BUFFER_SIZE);
	if (found > 0) {
		prop.skill = std::string(buffer);
		parsed = true;
	}
	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, JOB_POSITION, buffer, BUFFER_SIZE);
	if (found > 0) {
		prop.job_position = std::string(buffer);
		parsed = true;
	}
	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, GEOLOCATION, buffer, BUFFER_SIZE);
	if (found > 0) {
		prop.geolocation = std::string(buffer);
		parsed = true;
	}
	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, DISTANCE, buffer, BUFFER_SIZE);
	if (found > 0) {
		prop.distance = strtof(std::string(buffer).c_str(), 0);
		parsed = true;
	}
	return parsed;
}

Json HttpParser::parse_json_from_body(struct http_message* msg) {
	std::string json_string(msg->body.p, msg->body.len);
	std::string err;
	Json data = Json::parse(json_string, err);

	if (!err.empty()){
		throw InvalidJsonException();
	}
	return data;
}
