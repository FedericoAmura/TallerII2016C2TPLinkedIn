/*
 * API_Server.h
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_SERVER_API_SERVER_H_
#define APPSERVER_INCLUDE_SERVER_API_SERVER_H_

#include <string>

static const std::string APP_SERVER_URL		= "http://127.0.0.1";
#define DEFAULT_PORT	"8888"

/* Google Cloud Messaging */
static const std::string GOOGLE_CLOUD_MESSAGING_URL = "https://android.googleapis.com/gcm/send";
static const std::string GOOGLE_API_KEY_APPSERVER =  "AIzaSyCbDM4pvsecoAb4tvZwQzK6mvXjOU459gI";

/* URIs */
/* User */
static const std::string USER_ID = "\\d{1,}";
static const std::string DESCRIPTION = "[a-zA-Z]{1,}";

static const std::string LOGIN_URI				= "/login";
static const std::string SIGNUP_URI				= "/signup";
static const std::string USERS_URI				= "/users/";		// ej: /users/?cat=er&job=sas; (PARA BUSQUEDAS)
static const std::string USER_PROFILE_URI		= "/users/" + USER_ID;
static const std::string USER_SUMMARY_URI		= "/users/" + USER_ID + "/resume";
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
static const std::string CHAT_INC_MSG_URI		= "/chat/" + USER_ID + "/" + USER_ID + "/";		// ej: "/chat/123/456/?start=a&end=b"

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
static const int STATUS_FORBIDDEN		= 403;
static const int STATUS_NOT_FOUND 		= 404;
static const int STATUS_MET_NOT_ALLOWED	= 405;
static const int STATUS_NOT_ACCEPTABLE	= 406;
static const int STATUS_CONFLICT		= 409;
static const int STATUS_UNPROCESSABLE	= 422;
static const int STATUS_INT_SERVER_ERR	= 500;

/* Error Codes */
// for BAD REQUEST (400)
static const int ERR_CODE_NONEXISTENT_CAT   = 1;    /* nonexistent category */
static const int ERR_CODE_NONEXISTENT_SKILL = 2;    /* nonexistent skill */
static const int ERR_CODE_NONEXISTENT_JOB   = 3;    /* nonexistent job position */
static const int ERR_CODE_MALFORMED_LOC     = 4;    /* malformed location */
static const int ERR_CODE_INVALID_DISTANCE  = 5;    /* invalid distance */
static const int ERR_CODE_INVALID_POPSORT   = 6;    /* invalid popsort */

// for UNPROCESSABLE (422)
static const int ERR_CODE_INV_DATA_FORMAT   = 0;    /* invalid data format, i.e invalid json format */
static const int ERR_CODE_USRNM_UNAVAILABLE = 1;    /* username unavailable */
static const int ERR_CODE_INVALID_EMAIL     = 2;    /* invalid email */
static const int ERR_CODE_MALFORMED_DATE    = 3;    /* malformed date */
static const int ERR_CODE_LOC_OUT_OF_RANGE  = 4;    /* location out of range */
static const int ERR_CODE_BAD_PASSWORD_SIZE = 5;    /* bad password size */
static const int ERR_CODE_PHOTO_TOO_BIG     = 6;    /* photo too big */

/* Error Descriptions */
// for BAD REQUEST (400)
static const std::string ERR_DESC_NONEXISTENT_CAT   = "nonexistent category";
static const std::string ERR_DESC_NONEXISTENT_SKILL = "nonexistent skill";
static const std::string ERR_DESC_NONEXISTENT_JOB   = "nonexistent job position";
static const std::string ERR_DESC_MALFORMED_LOC     = "malformed location";
static const std::string ERR_DESC_INVALID_DISTANCE  = "invalid distance";
static const std::string ERR_DESC_INVALID_POPSORT   = "invalid popsort";

// for UNPROCESSABLE (422)
static const std::string ERR_DESC_INV_DATA_FORMAT   = "invalid data format";
static const std::string ERR_DESC_USRNM_UNAVAILABLE = "username unavailable";
static const std::string ERR_DESC_INVALID_EMAIL     = "invalid email";
static const std::string ERR_DESC_MALFORMED_DATE    = "malformed date";
static const std::string ERR_DESC_LOC_OUT_OF_RANGE  = "bad location";
static const std::string ERR_DESC_BAD_PASSWORD_SIZE = "bad password size";
static const std::string ERR_DESC_PHOTO_TOO_BIG     = "photo too big";

//for INT_SERVER_ERR (500)
static const std::string ERR_DESC_OPERATION_FAILED  = "operation failed";

#endif  // APPSERVER_INCLUDE_SERVER_API_SERVER_H_
