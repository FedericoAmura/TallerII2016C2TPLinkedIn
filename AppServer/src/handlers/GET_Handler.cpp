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
			res = handleUsers();
			break;
		case _USER_PROFILE:
			// /users/<user_id>
			res = handleUserProfile();
			break;
		case _USER_SUMMARY:
			// /users/<user_id>/summary
			res = handleUserSummary();
			break;
		case _USER_PHOTO:
			// /users/<user_id>/photo
			res = handleUserPhoto();
			break;
		case _USER_THUMB:
			// /users/<user_id>/thumb
			res = handleUserThumb();
			break;
		case _USER_BIEF:
			// /users/<user_id>/bief
			res = handleUserBrief();
			break;
		case _USERS_NOTIF:
			// /users/<user_id>/notif
			res = handleUsersNotif();
			break;
		case _USERS_NEW_NOTIF:
			// /users/<user_id>/notif/new
			res = handleUsersNewNotif();
			break;
		case _USERS_REQ_CONTACT:
			// /users/<user_id1>/notif/<user_id2>
			res = handleUsersNotifyRequest();
			break;
		case _USERS_CONTACTS:
			// /users/<user_id>/contacts
			res = handleUsersContacts();
			break;
		case _USERS_ADM_CONTACTS:
			// /users/<user_id1>/contacts/<user_id2>
			res = handleUsersContactManagement();
			break;
		case _POPULAR:
			// /users/popular
			res = handlePopular();
			break;
		case _POPULAR_RECOMMEND:
			// /users/popular/<user_id1>/<user_id2>
			res = handlePopularRecommend();
			break;
		case _POPULAR_POS:
			// /users/popular/position/<position>
			res = handlePopularByPosition();
			break;
		case _POPULAR_SKILL:
			// /users/popular/skill/<skill>
			res = handlePopularBySkill();
			break;
		case _CHAT_NEW:
			// /chat/<user_id>/new
			res = handleChatNew();
			break;
		case _CHAT_LAST_MSG:
			// /chat/<user_id>/last
			res = handleChatLastMsg();
			break;
		case _CHAT_INC_MSG:
			// /chat/<user_id1>/<user_id2>/?ini=a&fin=b
			res = handleChatIncMsg();
			break;
		case _CATEGORIES:
			// /categories
			res = handleCategories();
			break;
		case _JOB_POS_BY_CAT:
			// /job_positions/caterories/<category>
			res = handleJobPositionsByCategory();
			break;
		case _JOB_POSITION:
			// /job_positions/<job_position>
			res = handleJobPosition();
			break;
		case _SKILLS_BY_CAT:
			// /skills/categories/<category>
			res = handleSkillsByCategory();
			break;
		case _SKILL:
			// /skills/<skill>
			res = handleSkill();
			break;
		default:
			std::cout << "ERROR >> Method Not Allowed" << std::endl;
			return http_response("", STATUS_MET_NOT_ALLOWED);
			break;
	}
	return res;
}

http_response GET_Handler::handleUsers() {
	return http_response("{\"msg\":\"Users\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUserProfile() {
	return http_response("{\"msg\":\"Profile\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUserSummary() {
	return http_response("{\"msg\":\"Summary\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUserPhoto() {
	return http_response("{\"msg\":\"Photo\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUserThumb() {
	return http_response("{\"msg\":\"Thumb\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUserBrief() {
	return http_response("{\"msg\":\"Bief\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersNotif() {
	return http_response("{\"msg\":\"Notif\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersNewNotif() {
	return http_response("{\"msg\":\"New Notif\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersNotifyRequest() {
	return http_response("{\"msg\":\"Notify Request\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersContacts() {
	return http_response("{\"msg\":\"Contacts\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersContactManagement() {
	return http_response("{\"msg\":\"Contact Management\"}\n", STATUS_OK);
}

http_response GET_Handler::handlePopular() {
	return http_response("{\"msg\":\"{set Popular}\"}\n", STATUS_OK);
}

http_response GET_Handler::handlePopularRecommend() {
	return http_response("{\"msg\":\"{set Recommended Popular}\"}\n", STATUS_OK);
}

http_response GET_Handler::handlePopularByPosition() {
	return http_response("{\"msg\":\"{Popular by position}\"}\n", STATUS_OK);
}

http_response GET_Handler::handlePopularBySkill() {
	return http_response("{\"msg\":\"{Popular by Skill}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleChatNew() {
	return http_response("{\"chat\":\"{new chat}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleChatLastMsg() {
	return http_response("{\"chat\":\"{last msg}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleChatIncMsg() {
	return http_response("{\"chat\":\"{including msg}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleCategories() {
	return http_response("{\"categories\":\"{categories}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleJobPositionsByCategory() {
	return http_response("{\"job_positions\":\"{job positions by category}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleJobPosition() {
	return http_response("{\"job_position\":\"{job position}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleSkillsByCategory() {
	return http_response("{\"skills\":\"{skills by category}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleSkill() {
	return http_response("{\"skill\":\"{skill}\"}\n", STATUS_OK);
}

GET_Handler::~GET_Handler() {
}
