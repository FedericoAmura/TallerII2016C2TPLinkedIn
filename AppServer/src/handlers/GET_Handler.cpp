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
			res = handle_get_special_request();
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
			std::cout << "ERROR >> Method Not Allowed" << std::endl;
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
		std::cout << "Error: Non existent user. Query failed." << std::endl;
		return http_response("", STATUS_BAD_REQUEST);
	}

	return http_response(data.dump(), STATUS_OK);
}

http_response GET_Handler::handle_get_user_resume() {
	return http_response("{\"msg\":\"Summary\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_user_photo() {
	return http_response("{\"msg\":\"Photo\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_user_thumb() {
	return http_response("{\"msg\":\"Thumb\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_user_brief() {
	return http_response("{\"msg\":\"Bief\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_pending_contact_requests() {
	return http_response("{\"msg\":\"Notif\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_number_pending_requests() {
	return http_response("{\"msg\":\"New Notif\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_special_request() {
	return http_response("{\"msg\":\"Notify Request\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_user_contacts() {
	return http_response("{\"msg\":\"Contacts\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_are_they_connected() {
	return http_response("{\"msg\":\"Contact Management\"}\n", STATUS_OK);
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
	return http_response("{\"categories\":\"{categories}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_jobpositions_by_category() {
	return http_response("{\"job_positions\":\"{job positions by category}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_jobposition() {
	return http_response("{\"job_position\":\"{job position}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_skills_by_category() {
	return http_response("{\"skills\":\"{skills by category}\"}\n", STATUS_OK);
}

http_response GET_Handler::handle_get_skill() {
	return http_response("{\"skill\":\"{skill}\"}\n", STATUS_OK);
}

GET_Handler::~GET_Handler() {
}
