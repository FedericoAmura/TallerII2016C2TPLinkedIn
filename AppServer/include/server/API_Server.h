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

/* AppServer */
#define LOGIN 			"/login"
#define SIGNUP			"/signup"
#define RECOMMEND		"/recommend"
#define ADD_CONTACT		"/add_contact"
#define PROFESSIONALS	"/professionals"
#define POPULAR			"/popular"
#define PROFILE			"/profile"
#define CHAT			"/chat"

/* SharedServer */
#define CATEGORIES  	"/categories"
#define JOB_POSITIONS 	"/job_positions"
#define SKILLS 			"/skills"


#define GET		"GET"
#define POST 	"POST"
#define DELETE 	"DELETE"
#define PUT 	"PUT"

#define STATUS_OK			200
#define STATUS_CREATED		201
#define STATUS_NO_CONTENT	204
#define STATUS_BAD_REQUEST	400
#define STATUS_NOT_FOUND	404

enum URI {_LOGIN, _SIGNUP, _RECOMMEND, _ADD_CONTACT, _PROFESSIONALS, _POPULAR, _PROFILE, _CHAT, _INVALID_URI};

static URI validate_uri(std::string uri){
	if (uri == LOGIN) 		 return _LOGIN;
	if (uri == SIGNUP) 		 return _SIGNUP;
	if (uri == RECOMMEND) 	 return _RECOMMEND;
	if (uri == ADD_CONTACT)  return _ADD_CONTACT;
	if (uri == PROFESSIONALS)return _PROFESSIONALS;
	if (uri == POPULAR)		 return _POPULAR;
	if (uri == PROFILE)		 return _PROFILE;
	if (uri == CHAT) 		 return _CHAT;
	return _INVALID_URI;
}

#endif  // APPSERVER_INCLUDE_SERVER_API_SERVER_H_
