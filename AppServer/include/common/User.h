/*
 * User.h
 *
 *  Created on: Oct 6, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_COMMON_USER_H_
#define APPSERVER_INCLUDE_COMMON_USER_H_

#include <string>
#include <vector>

struct user_record {
	std::string first_name;
	std::string last_name;
	std::string birth;
	std::string email;
	std::string username;
	std::string password;
	std::string city;
	float longitude;
	float latitude;

	user_record() {
		first_name = "";
		last_name = "";
		birth = "";
		email = "";
		username = "";
		password = "";
		city = "";
		longitude = 0.0;
		latitude = 0.0;
	}
};

struct job_position {
	std::string name;
	std::string start;
	std::string end;

	job_position(std::string name, std::string start, std::string end) {
		this->name = name;
		this->start = start;
		this->end = end;
	}
};

struct user_update {
	std::string name;
	std::vector<std::string> skills;
	std::vector<job_position> job_positions;
	std::string city;
	std::string contacts;

	user_update() {
		name = "";
		skills.clear();
		job_positions.clear();
		city = "";
		contacts = "";
	}
};

struct contact_request {
	std::string sender_id;
	std::string target_id;
	std::string message;

	contact_request() {
		sender_id = "";
		target_id = "";
		message = "";
	}
};

struct recommend_request {
	std::string recommender;
	std::string recommended;
	bool recommends;

	recommend_request() {
		recommender = "";
		recommended = "";
		recommends = false;
	}
};

struct user_properties {
	std::string category;
	std::string skill;
	std::string job_position;
	std::string geolocation;
	float distance;
};
#endif /* APPSERVER_INCLUDE_COMMON_USER_H_ */
