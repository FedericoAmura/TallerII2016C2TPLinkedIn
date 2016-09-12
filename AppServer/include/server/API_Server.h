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

#define SIGNIN 			"/signin"
#define SIGNUP			"/signup"
#define CATEGORIES  	"/categories"
#define JOB_POSITIONS 	"/job_positions"
#define SKILLS 			"/skills"

#define GET		"GET"
#define POST 	"POST"
#define DELETE 	"DELETE"
#define PUT 	"PUT"

static bool validate_uri(std::string uri){
	if (uri == SIGNIN ||
		uri == SIGNUP ||
		uri == CATEGORIES ||
		uri == JOB_POSITIONS ||
		uri == SKILLS)
		return true;
	return false;
}

#endif  // APPSERVER_INCLUDE_SERVER_API_SERVER_H_
