/*
 * GETHandler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/GET_Handler.h"
#include <iostream>

GET_Handler::GET_Handler(http_request* req) : HTTPRequestHandler(req) {
}

http_response GET_Handler::handleRequest() {
	http_response res;
	switch (uri){
		case _USERS:
			// /users/?category=...&skill=...
			res = handle_get_search_for_users();
			break;
		case _USER_PROFILE:
			// /users/<user_id>
			res = handle_get_user_profile();
			break;
		case _USER_SUMMARY:
			// /users/<user_id>/summary
			res = handle_get_user_resume();
			break;
		case _USER_PHOTO:
			// /users/<user_id>/photo
			res = handle_get_user_photo();
			break;
		case _USER_THUMB:
			// /users/<user_id>/thumb
			res = handle_get_user_thumb();
			break;
		case _USER_BRIEF:
			// /users/<user_id>/brief
			res = handle_get_user_brief();
			break;
		case _USERS_NOTIF:
			// /users/<user_id>/notif
			res = handle_get_pending_contact_requests();
			break;
		case _USERS_NEW_NOTIF:
			// /users/<user_id>/notif/new
			res = handle_get_number_pending_requests();
			break;
		case _USERS_REQ_CONTACT:
			// /users/<user_id1>/notif/<user_id2>
			res = handle_get_particular_request();
			break;
		case _USERS_CONTACTS:
			// /users/<user_id>/contacts
			res = handle_get_user_contacts();
			break;
		case _USERS_ADM_CONTACTS:
			// /users/<user_id1>/contacts/<user_id2>
			res = handle_get_are_they_connected();
			break;
		case _POPULAR:
			// /users/popular
			res = handle_get_popular();
			break;
		case _POPULAR_RECOMMEND:
			// /users/popular/<user_id1>/<user_id2>
			res = handle_get_user_recommendations();
			break;
		case _POPULAR_POS:
			// /users/popular/position/<position>
			res = handle_get_popular_by_position();
			break;
		case _POPULAR_SKILL:
			// /users/popular/skill/<skill>
			res = handle_get_popular_by_skill();
			break;
		case _CHAT_NEW:
			// /chat/<user_id>/new
			res = handle_get_number_new_messages();
			break;
		case _CHAT_LAST_MSG:
			// /chat/<user_id>/<user_id2>/last
			res = handle_get_id_last_message();
			break;
		case _CHAT_INC_MSG:
			// /chat/<user_id1>/<user_id2>/?ini=a&fin=b
			res = handle_get_including_messages();
			break;
		case _CATEGORIES:
			// /categories
			res = handle_get_categories();
			break;
		case _JOB_POS_BY_CAT:
			// /job_positions/caterories/<category>
			res = handle_get_jobpositions_by_category();
			break;
		case _JOB_POSITION:
			// /job_positions/<job_position>
			res = handle_get_jobposition();
			break;
		case _SKILLS_BY_CAT:
			// /skills/categories/<category>
			res = handle_get_skills_by_category();
			break;
		case _SKILL:
			// /skills/<skill>
			res = handle_get_skill();
			break;
		default:
			std::cout << "[Error] Method Not Allowed" << std::endl;
			return http_response("", STATUS_MET_NOT_ALLOWED);
			break;
	}
	return res;
}

http_response GET_Handler::handle_get_search_for_users() {
	return http_response("{\"msg\":\"Users\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_user_profile() {
	// /users/<user_id>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);

	Json data;
	try {
		data = db_json->getDatos(userID);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent user. Query (Profile) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}

	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_user_resume() {
	// /users/<user_id>/resume
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);

	Json data;
	try {
		data = db_json->getResumen(userID);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent user. Query (Resume) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_user_photo() {
	// /users/<user_id>/photo
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);

	Json data;
	try {
		data = db_json->getFoto(userID);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent user. Query (Photo) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	} catch (LevelDBException &e) {
		std::cout << "[Error] User without photo. Query (Photo) failed." << std::endl;
		return http_response("", STATUS_NO_CONTENT);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_user_thumb() {
	// /users/<user_id>/thumb
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);

	Json data;
	try {
		data = db_json->getFotoThumbnail(userID);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent user. Query (Thumbnail) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	} catch (LevelDBException &e) {
		std::cout << "[Error] User without thumbail photo. Query (Thumbnail) failed." << std::endl;
		return http_response("", STATUS_NO_CONTENT);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_user_brief() {
	// /users/<user_id>/brief
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);

	Json data;
	try {
		data = db_json->getDatosBrief(userID);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent user. Query (Brief) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_pending_contact_requests() {
	// /users/<user_id>/notif
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);

	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[Error] Token not found. User unauthorized. Query (Pending requests) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[Error] Invalid token. Non existent token. Query (Pending requests) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[Error] Invalid token. Token has expired. Query (Pending requests) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data;
	try {
		data = db_json->getPeticionesPendientes(userID);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent user. Query (Pending requests) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_number_pending_requests() {
	// /users/<user_id>/notif/new
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);

	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[Error] Token not found. User unauthorized. Query (Number Pending requests) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[Error] Invalid token. Non existent token. Query (Number Pending requests) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[Error] Invalid token. Token has expired. Query (Number Pending requests) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data;
	try {
		data = db_json->getNumPeticionesPendientes(userID);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent user. Query (Number Pending requests) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_particular_request() {
	// /users/<user_id1>/notif/<user_id2>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID1 = std::stoi(vec_uri[1]);
	uint32_t userID2 = std::stoi(vec_uri[3]);

	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[Error] Token not found. User unauthorized. Query (Particular request) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[Error] Invalid token. Non existent token. Query (Particular request) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[Error] Invalid token. Token has expired. Query (Particular request) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data;
	try {
		data = db_json->getPeticion(userID1, userID2);
	} catch (NonexistentRequest &e) {
		std::cout << "[Error] Non existent request. Query (Particular request) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_user_contacts() {
	// /users/<user_id>/contacts
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);

	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[Error] Token not found. User unauthorized. Query (Particular request) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[Error] Invalid token. Non existent token. Query (Contacts) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[Error] Invalid token. Token has expired. Query (Contacts) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data;
	try {
		data = db_json->getContactos(userID);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent userID. Query (Contacts) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_are_they_connected() {
	// /users/<user_id1>/contacts/<user_id2>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID1 = std::stoi(vec_uri[1]);
	uint32_t userID2 = std::stoi(vec_uri[3]);

	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[Error] Token not found. User unauthorized. Query (Particular request) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[Error] Invalid token. Non existent token. Query (Contacts) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[Error] Invalid token. Token has expired. Query (Contacts) failed." << std::endl;
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data;
	bool connected;
	try {
		connected = db_json->esContacto(userID1, userID2);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] Non existent userID. Query (Are They Connected?) failed." << std::endl;
		return http_response("", STATUS_NOT_FOUND);
	}
	if (!connected)
		return http_response("", STATUS_NOT_FOUND);
	return http_response("", STATUS_NO_CONTENT);
}

http_response GET_Handler::handle_get_popular() {
	return http_response("{\"msg\":\"{set Popular}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_user_recommendations() {
	return http_response("{\"msg\":\"{set Recommended Popular}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_popular_by_position() {
	return http_response("{\"msg\":\"{Popular by position}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_popular_by_skill() {
	return http_response("{\"msg\":\"{Popular by Skill}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_number_new_messages() {
	return http_response("{\"chat\":\"{new chat}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_id_last_message() {
	return http_response("{\"chat\":\"{last msg}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_including_messages() {
	return http_response("{\"chat\":\"{including msg}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_categories() {
	Json data;
	try {
		data = SharedServerConnector::get_categories();
	} catch (CurlInitException &e) {
		std::cout << "[Error] Curl: init failed. Query (categories) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (CurlGetException &e) {
		std::cout << "[Error] Curl: GET failed. Query (categories) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (InvalidJsonException &e) {
		std::cout << "[Error] Invalid data received from Shared Server. Query (categories) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	}
	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_jobpositions_by_category() {
	// /job_positions/categories/<category>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string category = vec_uri[2];
	Json data;
	try {
		data = SharedServerConnector::get_job_positions();
	} catch (CurlInitException &e) {
		std::cout << "[Error] Curl: init failed. Query (particular job position) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (CurlGetException &e) {
		std::cout << "[Error] Curl: GET failed. Query (particular job position) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (InvalidJsonException &e) {
		std::cout << "[Error] Invalid data received from Shared Server. Query (particular job position) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	}

	std::vector<Json> job_positions_found;
	for (Json j : data["job_positions"].array_items()) {
		if (j["category"].string_value() == category)
			job_positions_found.push_back(j);
	}
	if (job_positions_found.empty())
		return http_response("", STATUS_NOT_FOUND);
	Json resp = Json::object{{"skills", Json::array(job_positions_found)}};
	return http_response(resp.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_jobposition() {
	// /job_positions/<job_position>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string job_pos = vec_uri[1];
	Json data;
	try {
		data = SharedServerConnector::get_job_positions();
	} catch (CurlInitException &e) {
		std::cout << "[Error] Curl: init failed. Query (particular job position) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (CurlGetException &e) {
		std::cout << "[Error] Curl: GET failed. Query (job position) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (InvalidJsonException &e) {
		std::cout << "[Error] Invalid data received from Shared Server. Query (job position) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	}

	for (Json j : data["job_positions"].array_items())
		if (j["name"].string_value() == job_pos)
			return http_response(j.dump(), STATUS_OK);

	return http_response("", STATUS_NOT_FOUND);
}

http_response GET_Handler::handle_get_skills_by_category() {
	// /skills/categories/<category>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string category = vec_uri[2];
	Json data;
	try {
		data = SharedServerConnector::get_skills();
	} catch (CurlInitException &e) {
		std::cout << "[Error] Curl: init failed. Query (particular skill) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (CurlGetException &e) {
		std::cout << "[Error] Curl: GET failed. Query (particular skill) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (InvalidJsonException &e) {
		std::cout << "[Error] Invalid data received from Shared Server. Query (particular skill) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	}

	std::vector<Json> skills_found;
	for (Json j : data["skills"].array_items()) {
		if (j["category"].string_value() == category)
			skills_found.push_back(j);
	}
	if (skills_found.empty())
		return http_response("", STATUS_NOT_FOUND);
	Json resp = Json::object{{"skills", Json::array(skills_found)}};
	return http_response(resp.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_skill() {
	// /skills/<skill>
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	std::string skill = vec_uri[1];
	Json data;
	try {
		data = SharedServerConnector::get_skills();
	} catch (CurlInitException &e) {
		std::cout << "[Error] Curl: init failed. Query (particular skill) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (CurlGetException &e) {
		std::cout << "[Error] Curl: GET failed. Query (particular skill) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	} catch (InvalidJsonException &e) {
		std::cout << "[Error] Invalid data received from Shared Server. Query (particular skill) failed." << std::endl;
		return http_response("", STATUS_INT_SERVER_ERR);
	}

	for (Json j : data["skills"].array_items())
		if (j["name"].string_value() == skill)
			return http_response(j.dump(), STATUS_OK);

	return http_response("", STATUS_NOT_FOUND);
}

GET_Handler::~GET_Handler() {
}
