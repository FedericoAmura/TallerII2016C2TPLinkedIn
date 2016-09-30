/*
 * API_Server.h
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_SERVER_API_SERVER_H_
#define APPSERVER_INCLUDE_SERVER_API_SERVER_H_

#define SHARED_SERVER_URL "http://127.0.0.1:5000"

#define APP_SERVER_URL	"http://127.0.0.1"
#define DEFAULT_PORT	"8888"

/* URIs */
#define USERS_URI			"/users"
#define RECOMMEND_URI		"/users/recommend"	 //TODO ver si se puede mejorar
#define ADD_CONTACT_URI		"/users/add_contact" //TODO ver si se puede mejorar
#define PROFESSIONALS_URI	"/users/professionals"
#define POPULAR_URI			"/users/popular"
#define CHAT_URI			"/chat"
#define CATEGORIES_URI  	"/categories"
#define JOB_POSITIONS_URI 	"/job_positions"
#define SKILLS_URI 			"/skills"

/* HTTP Methods */
#define GET		"GET"
#define POST 	"POST"
#define DELETE 	"DELETE"
#define PUT 	"PUT"

/* HTTP Status Codes */
#define STATUS_OK			200
#define STATUS_CREATED		201
#define STATUS_NO_CONTENT	204
#define STATUS_BAD_REQUEST	400
#define STATUS_NOT_FOUND	404

enum URI {_USERS, _RECOMMEND, _ADD_CONTACT, _PROFESSIONALS, _POPULAR, _CHAT, _JOB_POSITIONS, _SKILLS, _CATEGORIES, _INVALID_URI};

static URI validate_uri(std::string uri){
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

enum METHOD {_GET, _POST, _PUT, _DELETE, _INVALID_METHOD};

#endif  // APPSERVER_INCLUDE_SERVER_API_SERVER_H_
