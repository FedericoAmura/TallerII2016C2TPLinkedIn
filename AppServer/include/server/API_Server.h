/*
 * API_Server.h
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_SERVER_API_SERVER_H_
#define APPSERVER_INCLUDE_SERVER_API_SERVER_H_

#include <string>

static const std::string SHARED_SERVER_URL	= "http://jobifyg2.herokuapp.com"; //"http://127.0.0.1:5000"

static const std::string APP_SERVER_URL		= "http://127.0.0.1";
#define DEFAULT_PORT	"8888"

/* URIs */
/* User */
static const std::string USER_ID = "\\d{1,}";
static const std::string DESCRIPTION = "[a-z]{1,}";

static const std::string LOGIN_URI				= "/login";
static const std::string SIGNUP_URI				= "/signup";
static const std::string USERS_URI				= "/users/";		// ej: /users/123456/?cat=er&job=sas; (PARA BUSQUEDAS)
static const std::string USER_PROFILE_URI		= "/users/" + USER_ID;
static const std::string USER_SUMMARY_URI		= "/users/" + USER_ID + "/summary";
static const std::string USER_PHOTO_URI			= "/users/" + USER_ID + "/photo";
static const std::string USER_THUMB_URI			= "/users/" + USER_ID + "/thumb";
static const std::string USER_BRIEF_URI			= "/users/" + USER_ID + "/brief";

/* Contacts */
static const std::string USERS_NOTIF_URI		= "/users/" + USER_ID + "/notif";
static const std::string USERS_NEW_NOTIF_URI	= "/users/" + USER_ID + "/notif/new";
static const std::string USERS_REQ_NOTIF_URI	= "/users/" + USER_ID + "/notif/" + USER_ID;
static const std::string USERS_CONTACTS_URI		= "/users/" + USER_ID + "/contacts";
static const std::string USERS_REQ_CONTACTS_URI	= "/users/" + USER_ID + "/contacts/" + USER_ID;

/* Popularity / Recommendations */
static const std::string POPULAR_URI			= "/users/popular";
static const std::string POPULAR_RECOMMED_URI	= "/users/popular/" + USER_ID + "/" + USER_ID;
static const std::string POPULAR_POS_URI		= "/users/popular/position/" + DESCRIPTION;
static const std::string POPULAR_SKILL_URI		= "/users/popular/skill/" + DESCRIPTION;

/* Chat */
static const std::string CHAT_NEW_URI			= "/chat/" + USER_ID + "/new";
static const std::string CHAT_CHATS_URI			= "/chat/" + USER_ID + "/" + USER_ID;
static const std::string CHAT_LAST_MSG_URI		= "/chat/" + USER_ID + "/" + USER_ID + "/last";
static const std::string CHAT_INC_MSG_URI		= "/chat/" + USER_ID + "/" + USER_ID + "/";		// ej: "/chat/123/456/?ini=a&fin=b"

/* categories, job_positions, skills */
static const std::string JOB_POS_BY_CAT_URI		= "/job_positions/categories/" + DESCRIPTION;
static const std::string JOB_POS_URI			= "/job_positions/" + DESCRIPTION;
static const std::string SKILLS_BY_CAT_URI		= "/skills/categories/" + DESCRIPTION;
static const std::string SKILL_URI				= "/skills/" + DESCRIPTION;


/* SharedServer */
static const std::string CATEGORIES_URI			= "/categories";
static const std::string JOB_POSITIONS_URI		= "/job_positions";
static const std::string SKILLS_URI				= "/skills";

/* HTTP Methods */
#define GET		"GET"
#define POST	"POST"
#define DELETE	"DELETE"
#define PUT		"PUT"

/* HTTP Status Codes */
static const int STATUS_OK				= 200;
static const int STATUS_CREATED			= 201;
static const int STATUS_NO_CONTENT		= 204;
static const int STATUS_BAD_REQUEST		= 400;
static const int STATUS_UNAUTHORIZED	= 401;
static const int STATUS_NOT_FOUND 		= 404;
static const int STATUS_NOT_ACCEPTABLE	= 406;
static const int STATUS_CONFLICT		= 409;
static const int STATUS_UNPROCCESABLE	= 422;
static const int STATUS_INT_SERVER_ERR	= 500;

#endif  // APPSERVER_INCLUDE_SERVER_API_SERVER_H_
