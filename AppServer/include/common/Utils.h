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

enum METHOD {_GET, _POST, _PUT, _DELETE, _INVALID_METHOD};

enum URI {_LOGIN, _SIGNUP, _USERS, _RECOMMEND, _ADD_CONTACT, _PROFESSIONALS, _POPULAR, _CHAT, _JOB_POSITIONS, _SKILLS, _CATEGORIES, _INVALID_URI};

static URI validate_uri(std::string uri){
	if (uri == LOGIN_URI) 		 return _LOGIN;
	if (uri == SIGNUP_URI) 		 return _SIGNUP;
	if (uri == USERS_URI) 		 return _USERS;
	if (uri == RECOMMEND_URI) 	 return _RECOMMEND;
	if (uri == ADD_CONTACT_URI)  return _ADD_CONTACT;
	if (uri == PROFESSIONALS_URI)return _PROFESSIONALS;
	if (uri == POPULAR_URI)		 return _POPULAR;
	if (uri == CHAT_URI) 		 return _CHAT;
	if (uri == JOB_POSITIONS_URI)return _JOB_POSITIONS;
	if (uri == SKILLS_URI) 		 return _SKILLS;
	if (uri == CATEGORIES_URI) 	 return _CATEGORIES;
	return _INVALID_URI;
}



#endif /* APPSERVER_INCLUDE_COMMON_UTILS_H_ */
