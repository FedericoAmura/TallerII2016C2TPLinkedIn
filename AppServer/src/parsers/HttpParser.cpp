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

Json HttpParser::parse_search_for_users(struct http_message* msg) {
	Json::object data;
	int BUFFER_SIZE = 100;
	char buffer[BUFFER_SIZE];
	int found;

	found = mg_get_http_var(&msg->query_string, CATEGORY, buffer, BUFFER_SIZE);
	if (found > 0) {
		std::vector<std::string> categories = split(std::string(buffer), ";");
		Json::array array_categories;
		for (std::string category : categories)
			array_categories.push_back(category);
		data["categories"] = array_categories;
	}

	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, SKILL, buffer, BUFFER_SIZE);
	if (found > 0) {
		std::vector<std::string> skills = split(std::string(buffer), ";");
		Json::array array_skills;
		for (std::string skill : skills)
			array_skills.push_back(skill);
		data["skills"] = array_skills;
	}
	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, JOB_POSITION, buffer, BUFFER_SIZE);
	if (found > 0) {
		std::vector<std::string> positions = split(std::string(buffer), ";");
		Json::array array_positions;
		for (std::string position : positions)
			array_positions.push_back(position);
		data["job_positions"] = array_positions;
	}
	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, GEOLOCATION, buffer, BUFFER_SIZE);
	if (found > 0) {
		std::vector<std::string> coord = split(std::string(buffer), ";");
		double longitude = strtof(coord[0].c_str(), 0);
		double latitude = strtof(coord[1].c_str(), 0);
		data["longitude"] = longitude;
		data["latitude"] = latitude;
	}
	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, DISTANCE, buffer, BUFFER_SIZE);
	if (found > 0)
		data["distance"] = strtof(std::string(buffer).c_str(), 0);

	memset(buffer, 0, BUFFER_SIZE);
	found = mg_get_http_var(&msg->query_string, POPSORT, buffer, BUFFER_SIZE);
	if (found > 0) {
		if (std::string(buffer) == "True" || std::string(buffer) == "true")
			data["popsort"] = true;
		if (std::string(buffer) == "False" || std::string(buffer) == "false")
			data["popsort"] = false;
	}

	return Json(data);
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
