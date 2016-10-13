/*
 * JsonParser.cpp
 *
 *  Created on: Oct 6, 2016
 *      Author: emanuel
 */

#include "../../include/parsers/JsonParser.h"

json11::Json JsonParser::parseStringToJson(std::string data) {
	std::string err = "";
	json11::Json json = json11::Json::parse(data.c_str(), err);
	if (!err.empty())
		return json11::Json::object {};
	return json;
}


bool JsonParser::parse_user_data(struct user_record &rec, json11::Json data) {
	if (data[FIRST_NAME].is_null()) return false;
		rec.first_name = data[FIRST_NAME].string_value();

	if (data[LAST_NAME].is_null()) return false;
		rec.last_name = data[LAST_NAME].string_value();

	if (data[BIRTHDAY].is_null()) return false;
		rec.birth = data[BIRTHDAY].string_value();

	if (data[EMAIL].is_null()) return false;
		rec.email = data[EMAIL].string_value();

	if (data[USERNAME].is_null()) return false;
		rec.username = data[USERNAME].string_value();

	if (data[PASSWORD].is_null()) return false;
		rec.password = data[PASSWORD].string_value();

	if (data[CITY].is_null()) return false;
		rec.city = data[CITY].string_value();

	if (data[LONGITUDE].is_null()) return false;
		rec.longitude = strtof(data[LONGITUDE].string_value().c_str(), 0);

	if (data[LATITUDE].is_null()) return false;
		rec.latitude = strtof(data[LATITUDE].string_value().c_str(), 0);

	return true;
}

bool JsonParser::parse_user_update(struct user_update &rec, json11::Json data) {
	bool parsed = false;

	if (!data[NAME].is_null())
		rec.name = data[NAME].string_value();

	if (!data[USER_SKILLS].is_null()) {
		if (data[USER_SKILLS].is_array()) {
			json11::Json::array skills= data[USER_SKILLS].array_items();
			for (unsigned i = 0; i < skills.size(); i++)
				rec.skills.push_back(skills[i].string_value());
		}
	}

	if (!data[USER_JOB_POS].is_null()) {
		json11::Json::array job_positions = data[USER_JOB_POS].array_items();
		for (unsigned i = 0; i < job_positions.size(); i++) {
			std::string name = "", start = "", end = "";
			if (!job_positions[i][NAME].is_null()) name = job_positions[i][NAME].string_value();
			if (!job_positions[i][START].is_null()) start = job_positions[i][START].string_value();
			if (!job_positions[i][END].is_null()) end = job_positions[i][END].string_value();

			if (name != "" && start != "" && end != "")
				rec.job_positions.push_back(job_position(name, start, end));
		}
	}

	if (!data[CITY].is_null())
		rec.city = data[CITY].string_value();

	if (data[CONTACTS].is_null())
		rec.contacts = data[CONTACTS].string_value();

	if (rec.name != "" || !rec.skills.empty() || !rec.job_positions.empty() || rec.city != "" || rec.contacts != "")
		parsed = true;

	return parsed;
}

bool JsonParser::parse_parameter(std::string &buffer, const std::string param, json11::Json data) {
	buffer = "";

	if (!data[param].is_null() && data[param].is_string()) {
		buffer = data[param].string_value();
		return true;
	}

	return false;
}

bool JsonParser::parse_contact_request(struct contact_request &cont_req, json11::Json data) {
	if (data[SENDER_ID].is_null()) return false;
	if (data[TARGET_ID].is_null()) return false;

	cont_req.sender_id = data[SENDER_ID].string_value();
	cont_req.target_id = data[TARGET_ID].string_value();

	if (!data[MESSAGE].is_null())
		cont_req.message = data[MESSAGE].string_value();
	return true;
}

bool JsonParser::parse_recommend_request(struct recommend_request &recommend_req, json11::Json data) {
	if (data[RECOMMENDER].is_null()) return false;
	if (data[RECOMMENDED].is_null()) return false;
	if (data[RECOMMENDS].is_null()) return false;

	recommend_req.recommender = data[RECOMMENDER].string_value();
	recommend_req.recommended = data[RECOMMENDED].string_value();
	recommend_req.recommends = data[RECOMMENDS].bool_value();

	return true;
}
