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
			res = handleUsers();
			break;
		case _USER_PROFILE:
			res = handleUserProfile();
			break;
		case _USER_SUMMARY:
			res = handleUserSummary();
			break;
		case _USER_PHOTO:
			res = handleUserPhoto();
			break;
		case _USER_THUMB:
			res = handleUserThumb();
			break;
		case _USER_BIEF:
			res = handleUserBief();
			break;
		case _USERS_NOTIF:
			res = handleUsersNotif();
			break;
		case _USERS_NEW_NOTIF:
			res = handleUsersNewNotif();
			break;
		case _USERS_REQ_NOTIF:
			res = handleUsersReqNotif();
			break;
		case _USERS_CONTACTS:
			res = handleUsersContacts();
			break;
		case _USERS_REQ_CONTACTS:
			res = handleUsersReqContacts();
			break;
		case _POPULAR:
			res = handlePopular();
			break;
		case _POPULAR_RECOMMEND:
			res = handlePopularRecommend();
			break;
		case _POPULAR_POS:
			res = handlePopularByPosition();
			break;
		case _POPULAR_SKILL:
			res = handlePopularBySkill();
			break;
		case _CHAT_NEW:
			res = handleChatNew();
			break;
		case _CHAT_LAST_MSG:
			res = handleChatLastMsg();
			break;
		case _CHAT_INC_MSG:
			res = handleChatIncMsg();
			break;
		case _JOB_POSITIONS:
			res = handleJobPositions();
			break;
		case _JOB_POSITION:
			res = handleJobPosition();
			break;
		case _JOB_POSITION_CAT:
			res = handleJobPositionsByCategory();
			break;
		case _SKILLS:
			res = handleSkills();
			break;
		case _SKILL:
			res = handleSkill();
			break;
		case _SKILL_CAT:
			res = handleSkillsByCategory();
			break;
		case _CATEGORIES:
			res = handleCategories();
			break;
		default:
			res = http_response("", STATUS_BAD_REQUEST);
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

http_response GET_Handler::handleUserBief() {
	return http_response("{\"msg\":\"Bief\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersNotif() {
	return http_response("{\"msg\":\"Notif\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersNewNotif() {
	return http_response("{\"msg\":\"New Notif\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersReqNotif() {
	return http_response("{\"msg\":\"Req Notif\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersContacts() {
	return http_response("{\"msg\":\"Contacts\"}\n", STATUS_OK);
}

http_response GET_Handler::handleUsersReqContacts() {
	return http_response("{\"msg\":\"Req Contacts\"}\n", STATUS_OK);
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

http_response GET_Handler::handleJobPositions() {
/*
	std::string url = SHARED_SERVER_URL;
	url.append(JOB_POSITIONS_URI);
	auto response = cpr::Get(cpr::Url{url});
	if (response.status_code != STATUS_OK){
		sendReply("", STATUS_INT_SERVER_ERR);
		return;
	}
	sendReply(response.text, STATUS_OK);
*/
	return http_response("{\"job_positions\":\"{set JobPosition}\"}\n",STATUS_OK);
}

http_response GET_Handler::handleJobPosition() {
	return http_response("{\"job_position\":\"{job position}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleJobPositionsByCategory() {
	return http_response("{\"job_position\":\"{job position by category}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleSkills() {
/*
	std::string url = SHARED_SERVER_URL;
	url.append(SKILLS_URI);
	auto response = cpr::Get(cpr::Url{uri});
	if (response.status_code != STATUS_OK){
		sendReply("", STATUS_INT_SERVER_ERR);
		return;
	}
	sendReply(response.text, STATUS_OK);
*/
	return http_response("{\"skills\":\"{set Skills}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleSkill() {
	return http_response("{\"skill\":\"{skill}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleSkillsByCategory() {
	return http_response("{\"skills\":\"{skills by category}\"}\n", STATUS_OK);
}

http_response GET_Handler::handleCategories() {
/*
	std::string url = SHARED_SERVER_URL;
	url.append(CATEGORIES_URI);
	auto response = cpr::Get(cpr::Url{url});
	if (response.status_code != STATUS_OK){
		sendReply("", STATUS_INT_SERVER_ERR);
		return;
	}
	sendReply(response.text, STATUS_OK);
*/
	return http_response("{\"categories\":\"{set Categories}\"}\n", STATUS_OK);
}

GET_Handler::~GET_Handler() {
}
