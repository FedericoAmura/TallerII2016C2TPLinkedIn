#!/usr/bin/env python
# -*- coding: utf-8 -*-
import requests
import json

""" Estructura para test de un cliente
data = {
            "first_name" : "fulanito",
            "last_name" : "perez",
            "birth" : "11/2/2014",
            "email" : "mmmm@gmail.com"
            "username" : "cosmefulanito"
            "password" : "mi_password"
            "city" : "ciudad_perdida"
            "longitude" : "12.4"
            "latitude" : "3.4"
        }
"""
class Client:
    def __init__(self, data):
        self.data = data;
        self.userID = "000"
        self.token = "111"
        self.url_base = "http://127.0.0.1:8888" #AppServer (localhost con puerto 8888)

    def getAuthHeaderLogin(self):
        headers = {}
        headers["Content-Type"] = "application/json"
        auth_login = "username=" + self.data["username"] + " " + "password=" + self.data["password"]
        headers["Authorization"] = auth_login
        return headers

    def getAuthHeaderToken(self):
        headers = {}
        headers["Content-Type"] = "application/json"
        headers["Authorization"] = "token=" + self.token
        return headers

    def makeRequest_GET(self, url, params, headers):
        if (not bool(params)):
            return requests.get(url, data="", headers=headers)
        return requests.get(url, data="", params=params, headers=headers)

    def makeRequest_POST(self, url, data, headers):
        return requests.post(url, data= json.dumps(data), headers=headers)

    def makeRequest_PUT(self, url, data, headers):
        return requests.put(url, data= json.dumps(data), headers=headers)

    def makeRequest_DELETE(self, url, data, headers):
        return requests.delete(url, data= json.dumps(data), headers=headers)

#### POST   (data es un json, luego makeRequest_*** hace un dump del json)
    def login(self):
        url = self.url_base + "/login"
        headers = self.getAuthHeaderLogin()
        return self.makeRequest_POST(url, "", headers)

    def signup(self):
        url = self.url_base + "/signup"
        return self.makeRequest_POST(url, self.data, {})

    def accept_contact_request(self, other_userID):
        url = self.url_base + "/users/" + self.userID + "/notif/" + other_userID
        headers = self.getAuthHeaderToken()
        return self.makeRequest_POST(url, "", headers)

    def create_contact_request(self, data):
        url = self.url_base + "/users/" + self.userID + "/contacts"
        headers = self.getAuthHeaderToken()
        data["senderID"] = self.userID
        return self.makeRequest_POST(url, data, headers)

    def notify_msg_seen(self, data):
        url = self.url_base + "/chat/" + self.userID + "/new"
        headers = self.getAuthHeaderToken()
        data["userID"] = self.userID
        return self.makeRequest_POST(url, data, headers)

    def send_msg(self, data):
        other_userID = "123"
        url = self.url_base + "/chat/" + self.userID + "/" + other_userID
        headers = self.getAuthHeaderToken()
        data["senderID"] = self.userID
        return self.makeRequest_POST(url, data, headers)

### PUT     (data es un json, luego makeRequest_*** hace un dump del json)
    def update_profile(self, data):
        url = self.url_base + "/users/" + self.userID
        headers = self.getAuthHeaderToken()
        return self.makeRequest_PUT(url, data, headers)

    def update_summary(self, data):
        url = self.url_base + "/users/" + self.userID + "/summary"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_PUT(url, data, headers)

    def update_photo(self, data):
        url = self.url_base + "/users/" + self.userID + "/photo"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_PUT(url, data, headers)

    def recommend_user(self, receiverID, data):
        url = self.url_base + "/users/popular/" + self.userID + "/" + receiverID
        headers = self.getAuthHeaderToken()
        data["recommender"] = self.userID
        return self.makeRequest_PUT(url, data, headers)

### DELETE  (data es un json, luego makeRequest_*** hace un dump del json)
    def close_session(self):
        url = self.url_base + "/login"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_DELETE(url, "", headers)

    def reject_contact_request(self, other_userID):
        url = self.url_base + "/users/" + self.userID + "/notif/" + other_userID
        headers = self.getAuthHeaderToken()
        return self.makeRequest_DELETE(url, "", headers)

    def delete_contact(self, other_userID):
        url = self.url_base + "/users/" + self.userID + "/contacts/" + other_userID
        headers = self.getAuthHeaderToken()
        return self.makeRequest_DELETE(url, "", headers)

### GET     (params es un hash para agregar los parámetros que se desean mandar en la url)
###         (en algunos casos se necesitará agregar la autenticación en el header Authorization)
    def search_for_users(self, params):
        url = self.url_base + "/users/"
        return self.makeRequest_GET(url, params, {})

    def get_profile_from(self, other_userID):
        url = self.url_base + "/users/" + other_userID
        return self.makeRequest_GET(url, "", {})

    def get_summary_from(self, other_userID):
        url = self.url_base + "/users/" + other_userID + "/summary"
        return self.makeRequest_GET(url, "", {})

    def get_photo_from(self, other_userID):
        url = self.url_base + "/users/" + other_userID + "/photo"
        return self.makeRequest_GET(url, "", {})

    def get_photo_thumb_from(self, other_userID):
        url = self.url_base + "/users/" + other_userID + "/thumb"
        return self.makeRequest_GET(url, "", {})

    def get_info_brief_from(self, other_userID):
        url = self.url_base + "/users/" + other_userID + "/brief"
        return self.makeRequest_GET(url, "", {})

    def get_pending_contact_request(self):
        url = self.url_base + "/users/" + self.userID + "/notif"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_number_pending_requests(self):
        url = self.url_base + "/users/" + self.userID + "/notif/new"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_particuliar_request(self, other_userID):
        url = self.url_base + "/users/" + self.userID + "/notif/" + other_userID
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_contacts(self):
        url = self.url_base + "/users/" + self.userID + "/contacts"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_are_we_contacts(self, other_userID):
        url = self.url_base + "/users/" + self.userID + "/contacts/" + other_userID
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_number_new_messages(self):
        url = self.url_base + "/chat/" + self.userID + "/new"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_id_last_message(self, other_userID):
        url = self.url_base + "/chat/" + self.userID + "/" + other_userID + "/last"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_including_messages(self, other_userID, params):
        url = self.url_base + "/chat/" + self.userID + "/" + other_userID + "/"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, params, headers)

    def get_popular(self):
        url = self.url_base + "/users/popular"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_user_recommended_to_another_user(self, other_userID):
        url = self.url_base + "/users/popular/" + self.userID + "/" + other_userID
        return self.makeRequest_GET(url, "", {})

    def get_popular_by_position(self, position):
        url = self.url_base + "/users/popular/position/" + position
        return self.makeRequest_GET(url, "", {})

    def get_popular_by_skill(self, skill):
        url = self.url_base + "/users/popular/skill/" + skill
        return self.makeRequest_GET(url, "", {})

    def get_categories(self):
        url = self.url_base + "/categories"
        return self.makeRequest_GET(url, "", {})

    def get_job_position_by_category(self, category):
        url = self.url_base + "/job_positions/categories/" + category
        return self.makeRequest_GET(url, "", {})

    def get_job_position(self, job_position):
        url = self.url_base + "/job_positions/" + job_position
        return self.makeRequest_GET(url, "", {})

    def get_skills_by_category(self, category):
        url = self.url_base + "/skills/" + category
        return self.makeRequest_GET(url, "", {})

    def get_skill(self, skill):
        url = self.url_base + "/skills/" + skill
        return self.makeRequest_GET(url, "", {})
