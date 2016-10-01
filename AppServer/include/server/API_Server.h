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
#define LOGIN_URI			"/login"
#define SIGNUP_URI			"/signup"
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
#define STATUS_UNAUTHORIZED 401
#define STATUS_NOT_FOUND	404

#endif  // APPSERVER_INCLUDE_SERVER_API_SERVER_H_
