/*
 * PUT_Handler.cpp
 *
 *  Created on: Sep 30, 2016
 *      Author: emanuel
 */

#include "../../include/handlers/PUT_Handler.h"
#include <iostream>

PUT_Handler::PUT_Handler(http_request* req) : HTTPRequestHandler(req) {
}

http_response PUT_Handler::handleRequest() {
	http_response res;
	switch (uri){
		case _USER_PROFILE:
			// /users/<user_id>
			res = handle_update_profile();
			break;
		case _USER_SUMMARY:
			// /users/<user_id>/summary
			res = handle_update_resume();
			break;
		case _USER_PHOTO:
			// /users/<user_id>/photo
			res = handle_update_photo();
			break;
		case _POPULAR_RECOMMEND:
			// /users/popular/<user_id1>/<user_id2>
			res = handle_recommend_user();
			break;
		default:
			std::cout << "[WARN] Method Not Allowed" << std::endl;
			Logger::log(WARN, "Method not allowed.");
			return http_response("", STATUS_MET_NOT_ALLOWED);
			break;
	}
	return res;
}

http_response PUT_Handler::handle_update_profile() {
	/* uri = /users/<userID> */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[WARN] Token not found. User unauthorized. Update profile failed." << std::endl;
		Logger::log(WARN, "Token not found. User unauthorized. Update profile failed.");
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[WARN] Invalid token. Non existent token. Update profile failed." << std::endl;
		Logger::log(WARN, "Invalid token. Non existent token. Update profile failed.");
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[WARN] Invalid token. Token has expired. Update profile failed." << std::endl;
		Logger::log(WARN, "Invalid token. Token has expired. Update profile failed.");
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data, error;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		error = Json::object {{"error_code", ERR_CODE_INV_DATA_FORMAT}, {"description", ERR_DESC_INV_DATA_FORMAT}};
		std::cout << "[WARN] Invalid Json Format. Update Profile failed." << std::endl;
		Logger::log(WARN, "Invalid Json Format. Update Profile failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	}

	try {
		db_json->setDatos(userID, data);
	} catch (NonexistentUserID &e) {
		std::cout << "[WARN] Nonexistent userID. Update Profile failed." << std::endl;
		Logger::log(WARN, "Nonexistent userID. Update Profile failed.");
		return http_response("", STATUS_NOT_FOUND);
	} catch (NonexistentSkill &e) {
		error = Json::object {{"error_code", ERR_CODE_NONEXISTENT_SKILL}, {"description", ERR_DESC_NONEXISTENT_SKILL}};
		std::cout << "[WARN] Non existent skill. Update Profile failed." << std::endl;
		Logger::log(WARN, "Nonexistent skill. Update Profile failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	} catch (NonexistentPosition &e) {
		error = Json::object {{"error_code", ERR_CODE_NONEXISTENT_JOB}, {"description", ERR_DESC_NONEXISTENT_JOB}};
		std::cout << "Non existent position. Update Profile failed." << std::endl;
		Logger::log(WARN, "Nonexistent position. Update Profile failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	} catch (BadInputException &e) {
		error = Json::object {{"error_code", ERR_CODE_INV_DATA_FORMAT}, {"description", ERR_DESC_INV_DATA_FORMAT}};
		std::cout << "[WARN] \"" << e.what() << "\" not found. Update Profile failed." << std::endl;
		Logger::log(WARN, "\"" + std::string(e.what()) + "\" not found. Update Profile failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	}

	return http_response("{}", STATUS_NO_CONTENT);
}

http_response PUT_Handler::handle_update_resume() {
	/* uri = /users/<userID>/summary */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[WARN] Token not found. User unauthorized. Update summary failed." << std::endl;
		Logger::log(WARN, "Token not found. User unauthorized. Update summary failed." );
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[WARN] Invalid token. Non existent token. Update summary failed." << std::endl;
		Logger::log(WARN, "Invalid token. Non existent token. Update summary failed." );
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[WARN] Invalid token. Token has expired. Update summary failed." << std::endl;
		Logger::log(WARN, "Invalid token. Token has expired. Update summary failed." );
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data, error;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		error = Json::object {{"error_code", ERR_CODE_INV_DATA_FORMAT}, {"description", ERR_DESC_INV_DATA_FORMAT}};
		std::cout << "[WARN] Invalid Json Format. Update Summary failed." << std::endl;
		Logger::log(WARN, "Invalid Json Format. Update Summary failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	}

	try {
		db_json->setResumen(userID, data);
	} catch (NonexistentUserID &e) {
		std::cout << "[Error] UserID not found. Update summary failed." << std::endl;
		Logger::log(WARN, "UserID not found. Update summary failed.");
		return http_response("", STATUS_NOT_FOUND);
	} catch (BadInputException &e) {
		error = Json::object {{"error_code", ERR_CODE_INV_DATA_FORMAT}, {"description", ERR_DESC_INV_DATA_FORMAT}};
		std::cout << "[WARN] \"resume\" not found. Update Summary failed." << std::endl;
		Logger::log(WARN, "\"resume\" not found. Update Summary failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	}
	return http_response("{}", STATUS_NO_CONTENT);
}

http_response PUT_Handler::handle_update_photo() {
	/* uri = /users/<userID>/photo */
	std::vector<std::string> vec_uri = split(request->uri(), "/");
	uint32_t userID = std::stoi(vec_uri[1]);
	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[WARN] Token not found. User unauthorized. Update Photo failed." << std::endl;
		Logger::log(WARN, "Token not found. User unauthorized. Update photo failed.");
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[WARN] Invalid token. Non existent token. Update photo failed." << std::endl;
		Logger::log(WARN, "Invalid token. Non existent token. Update photo failed.");
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[WARN] Invalid token. Token has expired. Update photo failed." << std::endl;
		Logger::log(WARN, "Invalid token. Token has expired. Update photo failed.");
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data, error;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		error = Json::object {{"error_code", ERR_CODE_INV_DATA_FORMAT}, {"description", ERR_DESC_INV_DATA_FORMAT}};
		std::cout << "[WARN] Invalid Json Format. Update Photo failed." << std::endl;
		Logger::log(WARN, "Invalid Json Format. Update Photo failed." );
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	}

	try {
		db_json->setFoto(userID, data);
	} catch (NonexistentUserID &e) {
		std::cout << "[WARN] UserID not found. Update photo failed." << std::endl;
		Logger::log(WARN, "UserID not found. Update photo failed.");
		return http_response("", STATUS_NOT_FOUND);
	} catch (TooBigException &e) {
		error = Json::object {{"error_code", ERR_CODE_PHOTO_TOO_BIG}, {"description", ERR_DESC_PHOTO_TOO_BIG}};
		std::cout << "[WARN] Photo too big. Update photo failed." << std::endl;
		Logger::log(WARN, "Photo too big. Update photo failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	} catch (BadInputException &e) {
		error = Json::object {{"error_code", ERR_CODE_INV_DATA_FORMAT}, {"description", ERR_DESC_INV_DATA_FORMAT}};
		std::cout << "[WARN] \"photo\" not found. Update photo failed." << std::endl;
		Logger::log(WARN, "\"photo\" not found. Update photo failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	}
	return http_response("{}", STATUS_NO_CONTENT);
}

http_response PUT_Handler::handle_recommend_user() {
	/* uri = /users/popular/<userID1>/<userID2> */
//	std::vector<std::string> vec_uri = split(request->uri(), "/");
//	uint32_t userID1 = std::stoi(vec_uri[2]);
//	uint32_t userID2 = std::stoi(vec_uri[3]);

	std::string token;
	bool parsed = HttpParser::parse_variable_from_authorization_header(request->message, TOKEN, token);
	if (!parsed) {
		std::cout << "[WARN] Token not found. User unauthorized. Recommend user failed." << std::endl;
		Logger::log(WARN, "Token not found. User unauthorized. Recommend user failed." );
		return http_response("", STATUS_FORBIDDEN);
	}

	try {
		db_json->validar_token(token);
	} catch (NonexistentToken &e) {
		std::cout << "[WARN] Invalid token. Non existent token. Recommend user failed." << std::endl;
		Logger::log(WARN, "Invalid token. Non existent token. Recommend user failed.");
		return http_response("", STATUS_FORBIDDEN);
	}catch (TokenHasExpired &e) {
		std::cout << "[WARN] Invalid token. Token has expired. Recommend user failed." << std::endl;
		Logger::log(WARN, "Invalid token. Token has expired. Recommend user failed.");
		return http_response("", STATUS_FORBIDDEN);
	}

	Json data, error;
	try {
		data = HttpParser::parse_json_from_body(request->message);
	} catch (InvalidJsonException &e) {
		error = Json::object {{"error_code", ERR_CODE_INV_DATA_FORMAT}, {"description", ERR_DESC_INV_DATA_FORMAT}};
		std::cout << "[WARN] Invalid Json Format. Recommend user failed." << std::endl;
		Logger::log(WARN, "Invalid Json Format. Recommend user failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	}

	try {
		db_json->actualizarRecomendacion(data);
	} catch (NonexistentUserID &e) {
		std::cout << "[WARN] Non existent userID. Recommend user failed." << std::endl;
		Logger::log(WARN, "Nonexistent userID. Recommend user failed.");
		return http_response("", STATUS_NOT_FOUND);
	} catch (BadInputException &e) {
		error = Json::object {{"error_code", ERR_CODE_INV_DATA_FORMAT}, {"description", ERR_DESC_INV_DATA_FORMAT}};
		std::cout << "[WARN] \"" << e.what() << "\" not found. Recommend user failed." << std::endl;
		Logger::log(WARN, "\"" + std::string(e.what()) + "\" not found. Recommend user failed.");
		return http_response(error.dump(), STATUS_UNPROCESSABLE);
	}
	return http_response("{}", STATUS_NO_CONTENT);
}

PUT_Handler::~PUT_Handler() {
}
