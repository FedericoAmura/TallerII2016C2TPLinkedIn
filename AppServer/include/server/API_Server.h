/*
 * API_Server.h
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_SERVER_API_SERVER_H_
#define APPSERVER_INCLUDE_SERVER_API_SERVER_H_

#define SHARED_SERVER_URL "http://jobifyg2.herokuapp.com" //"http://127.0.0.1:5000"

#define APP_SERVER_URL	"http://127.0.0.1"
#define DEFAULT_PORT	"8888"

/* URIs */
/* User */
#define LOGIN_URI			"/login"
#define SIGNUP_URI			"/signup"
#define USERS_URI			"/users/"
#define USER_PROFILE_URI	"/users/.*" // TODO revisar ya que se utiliza para profiles
#define USER_SUMMARY_URI	"/users/.*/summary"
#define USER_PHOTO_URI		"/users/.*/photo"
#define USER_THUMB_URI		"/users/.*/thumb"
#define USER_BIEF_URI		"/users/.*/bief"

/* Contacts */
#define USERS_NOTIF_URI		"/users/.*/notif"
#define USERS_NEW_NOTIF_URI	"/users/.*/notif/new"
#define USERS_REQ_NOTIF_URI	"/users/.*/notif/.*"
#define USERS_CONTACTS_URI	"/users/.*/contacts"
#define USERS_REQ_CONTACTS_URI	"/users/.*/contacts/.*"

/* Popularity / Recommendations */
#define POPULAR_URI			"/users/popular"
#define POPULAR_RECOMMED_URI "/users/popular/.*/.*"
#define POPULAR_POS_URI		"/users/popular/position/.*"
#define POPULAR_SKILL_URI	"/users/popular/skill/.*"

/* Chat */
#define CHAT_NEW_URI		"/chat/.*/new"
#define CHAT_CHATS_URI		"/chat/.*/.*"
#define CHAT_LAST_MSG_URI	"/chat/.*/last"
#define CHAT_INC_MSG_URI	"/chat/.*/.*/?.*"

/* SharedServer */
#define CATEGORIES_URI  	"/categories"
#define JOB_POSITIONS_URI 	"/job_positions"
#define JOB_POSITION_URI	"/job_positions/.*"
#define JOB_POSITION_CAT_URI "/job_positions/categories/.*"
#define SKILLS_URI 			"/skills"
#define SKILL_URI	 		"/skills/.*"
#define SKILL_CAT_URI 		"/skills/categories/.*"

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
#define STATUS_NOT_ACCEPTABLE 406
#define STATUS_CONFLICT		409
#define STATUS_INT_SERVER_ERR 500

#endif  // APPSERVER_INCLUDE_SERVER_API_SERVER_H_
