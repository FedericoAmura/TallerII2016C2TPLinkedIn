/*
 * RequestHandler.h
 *
 *  Created on: Sep 4, 2016
 *      Author: emanuel
 */

#ifndef APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_
#define APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_

#include "../common/req_res.h"
#include "../handlers/DataBasesHandler.h"

class RequestHandler{
 private:
	http_request* request;
	URI uri;
	DataBasesHandler* db_handler;

	void handleGetRequest();
	void handlePostRequest();
	void handlePutRequest();
	void handleDeleteRequest();

	/* GET */
	void handleUsers_GET();
	void handleProfessionals_GET();
	void handlePopular_GET();
	void handleJobPositions_GET();
	void handleSkills_GET();
	void handleCategories_GET();

	/* POST */
	void handleUsers_POST();
	void handleRecommendations_POST();
	void handleAddContact_POST();
	void handleChat_POST();
	void handleJobPositions_POST();
	void handleSkills_POST();
	void handleCategories_POST();

	/* PUT */
	void handleUsers_PUT();
	void handleJobPositions_PUT();
	void handleSkills_PUT();

	/* DELETE */
	void handleJobPositions_DELETE();
	void handleSkills_DELETE();
	void handleCategories_DELETE();

	void handleLogIn();
	void handleSignUp();

	void sendReply(std::string res, int status);
	void sendBadRequest();

	void closeConnection();

	bool validToken();

 public:
	explicit RequestHandler(http_request* req);
	void handleRequest();
	virtual ~RequestHandler();
};

#endif  // APPSERVER_INCLUDE_HANDLERS_REQUESTHANDLER_H_
