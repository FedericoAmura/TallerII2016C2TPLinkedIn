/*
 * Utils.h
 *
 *  Created on: Oct 1, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_COMMON_UTILS_H_
#define APPSERVER_INCLUDE_COMMON_UTILS_H_

#include "../server/API_Server.h"
#include <string>
#include <string.h>
#include <vector>
#include <regex>

enum METHOD {_GET, _POST, _PUT, _DELETE, _INVALID_METHOD};

enum URI {	_LOGIN,
		 	_SIGNUP,
			_USERS,
			_USER_PROFILE,
			_USER_SUMMARY,
			_USER_PHOTO,
			_USER_THUMB,
			_USER_BRIEF,
			_USERS_NOTIF,
			_USERS_NEW_NOTIF,
			_USERS_REQ_CONTACT,
			_USERS_CONTACTS,
			_USERS_ADM_CONTACTS,
			_POPULAR,
			_POPULAR_RECOMMEND,
			_POPULAR_POS,
			_POPULAR_SKILL,
			_CHAT_NEW,
			_CHAT_CHATS,
			_CHAT_LAST_MSG,
			_CHAT_INC_MSG,
			_CATEGORIES,
			_JOB_POS_BY_CAT,
			_JOB_POSITION,
			_SKILLS_BY_CAT,
			_SKILL,
			_INVALID_URI};

static URI validate_uri(std::string uri){
	if (uri == LOGIN_URI) 		 return _LOGIN;
	if (uri == SIGNUP_URI) 		 return _SIGNUP;
	if (uri == USERS_URI) 		 return _USERS;
	if (std::regex_match(uri, std::regex(USER_PROFILE_URI)))return _USER_PROFILE;
	if (std::regex_match(uri, std::regex(USER_SUMMARY_URI)))return _USER_SUMMARY;
	if (std::regex_match(uri, std::regex(USER_PHOTO_URI))) 	return _USER_PHOTO;
	if (std::regex_match(uri, std::regex(USER_THUMB_URI))) 	return _USER_THUMB;
	if (std::regex_match(uri, std::regex(USER_BRIEF_URI))) 	return _USER_BRIEF;

	if (std::regex_match(uri, std::regex(USERS_NOTIF_URI)))		return _USERS_NOTIF;
	if (std::regex_match(uri, std::regex(USERS_NEW_NOTIF_URI)))	return _USERS_NEW_NOTIF;
	if (std::regex_match(uri, std::regex(USERS_REQ_NOTIF_URI)))	return _USERS_REQ_CONTACT;
	if (std::regex_match(uri, std::regex(USERS_CONTACTS_URI)))	return _USERS_CONTACTS;
	if (std::regex_match(uri, std::regex(USERS_REQ_CONTACTS_URI)))	return _USERS_ADM_CONTACTS;

	if (uri == POPULAR_URI)		 								return _POPULAR;
	if (std::regex_match(uri, std::regex(POPULAR_RECOMMED_URI)))return _POPULAR_RECOMMEND;
	if (std::regex_match(uri, std::regex(POPULAR_POS_URI)))		return _POPULAR_POS;
	if (std::regex_match(uri, std::regex(POPULAR_SKILL_URI)))	return _POPULAR_SKILL;

	if (std::regex_match(uri, std::regex(CHAT_NEW_URI))) 		return _CHAT_NEW;
	if (std::regex_match(uri, std::regex(CHAT_CHATS_URI))) 		return _CHAT_CHATS;
	if (std::regex_match(uri, std::regex(CHAT_LAST_MSG_URI))) 	return _CHAT_LAST_MSG;
	if (std::regex_match(uri, std::regex(CHAT_INC_MSG_URI))) 	return _CHAT_INC_MSG;

	if (std::regex_match(uri, std::regex(CATEGORIES_URI))) 		return _CATEGORIES;
	if (std::regex_match(uri, std::regex(JOB_POS_BY_CAT_URI))) 	return _JOB_POS_BY_CAT;
	if (std::regex_match(uri, std::regex(JOB_POS_URI))) 		return _JOB_POSITION;
	if (std::regex_match(uri, std::regex(SKILLS_BY_CAT_URI))) 	return _SKILLS_BY_CAT;
	if (std::regex_match(uri, std::regex(SKILL_URI))) 			return _SKILL;

	return _INVALID_URI;
}

static std::string MethodtoString(METHOD method) {
	if (method == _GET) return GET;
	if (method == _POST) return POST;
	if (method == _PUT) return PUT;
	if (method == _DELETE) return DELETE;
	return "INVALID METHOD";

}

static std::vector<std::string> split(std::string line, const char* separator) {
	std::vector<std::string> vec;
    char * dup = strdup(line.c_str());
    char * token = strtok(dup, separator);
    while(token != NULL){
        vec.push_back(std::string(token));
        token = strtok(NULL, separator);
    }
    free(dup);
	return vec;
}

#endif /* APPSERVER_INCLUDE_COMMON_UTILS_H_ */
