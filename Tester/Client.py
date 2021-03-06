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
        self.userID = -1
        self.token = ""
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

    def get_user_id(self):
        return self.userID

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

    def makeBadRequestWithJson_POST(self, ):
        url = self.url_base + "/users/" + str(self.userID) + "/notif/" + str(6)
        data = "{\"userID\":}"
        headers = self.getAuthHeaderToken()
        return requests.post(url, data=data, headers=headers)

#### POST   (data es un json, luego makeRequest_*** hace un dump del json)
    def login(self):
        url = self.url_base + "/login"
        headers = self.getAuthHeaderLogin()
        res = self.makeRequest_POST(url, "", headers)
        if (res.status_code == 200):
            data = json.loads(res.text)
#            print "[login] userID recibido: ", data["userID"]
#            print "[login] token recibido: ", data["token"]
            self.userID = data["userID"]
            self.token = data["token"]
        return res

    def signup(self):
        url = self.url_base + "/signup"
        return self.makeRequest_POST(url, self.data, {})

    def accept_contact_request(self, another_userID):
        url = self.url_base + "/users/" + str(self.userID) + "/notif/" + str(another_userID)
        headers = self.getAuthHeaderToken()
        return self.makeRequest_POST(url, "", headers)

    def create_contact_request(self, data):
        url = self.url_base + "/users/" + str(self.userID) + "/contacts"
        headers = self.getAuthHeaderToken()
        data["userID"] = self.userID
        return self.makeRequest_POST(url, data, headers)

    def notify_msg_seen(self, data):
        url = self.url_base + "/chat/" + str(self.userID) + "/new"
        headers = self.getAuthHeaderToken()
        data["userID"] = self.userID
        return self.makeRequest_POST(url, data, headers)

    def send_msg(self, data):
        url = self.url_base + "/chat/" + str(self.userID) + "/" + str(data["receiverID"])
        headers = self.getAuthHeaderToken()
        data["senderID"] = self.userID
        return self.makeRequest_POST(url, data, headers)

### PUT     (data es un json, luego makeRequest_*** hace un dump del json)
    def update_profile(self, data):
        url = self.url_base + "/users/" + str(self.userID)
        headers = self.getAuthHeaderToken()
        return self.makeRequest_PUT(url, data, headers)

    def update_resume(self, data):
        url = self.url_base + "/users/" + str(self.userID) + "/resume"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_PUT(url, data, headers)

    def update_photo(self, data):
        url = self.url_base + "/users/" + str(self.userID) + "/photo"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_PUT(url, data, headers)

    def recommend_user(self, data):
        url = self.url_base + "/users/popular/" + str(self.userID) + "/" + str(data["recommended"])
        headers = self.getAuthHeaderToken()
        data["recommender"] = self.userID
        return self.makeRequest_PUT(url, data, headers)

### DELETE  (data es un json, luego makeRequest_*** hace un dump del json)
    def logout(self):
        url = self.url_base + "/login"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_DELETE(url, "", headers)

    def reject_contact_request(self, another_userID):
        url = self.url_base + "/users/" + str(self.userID) + "/notif/" + str(another_userID)
        headers = self.getAuthHeaderToken()
        return self.makeRequest_DELETE(url, "", headers)

    def delete_contact(self, another_userID):
        url = self.url_base + "/users/" + str(self.userID) + "/contacts/" + str(another_userID)
        headers = self.getAuthHeaderToken()
        return self.makeRequest_DELETE(url, "", headers)

### GET     (params es un hash para agregar los parámetros que se desean mandar en la url)
###         (en algunos casos se necesitará agregar la autenticación en el header Authorization)
    def user_search(self, params):
        url = self.url_base + "/users/"
        return self.makeRequest_GET(url, params, {})

    def get_profile_from(self, another_userID):
        url = self.url_base + "/users/" + str(another_userID)
        return self.makeRequest_GET(url, "", {})

    def get_resume_from(self, another_userID):
        url = self.url_base + "/users/" + str(another_userID) + "/resume"
        return self.makeRequest_GET(url, "", {})

    def get_photo_from(self, another_userID):
        url = self.url_base + "/users/" + str(another_userID) + "/photo"
        return self.makeRequest_GET(url, "", {})

    def get_photo_thumb_from(self, another_userID):
        url = self.url_base + "/users/" + str(another_userID) + "/thumb"
        return self.makeRequest_GET(url, "", {})

    def get_info_brief_from(self, another_userID):
        url = self.url_base + "/users/" + str(another_userID) + "/brief"
        return self.makeRequest_GET(url, "", {})

    def get_pending_contact_request(self):
        url = self.url_base + "/users/" + str(self.userID) + "/notif"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_number_pending_requests(self):
        url = self.url_base + "/users/" + str(self.userID) + "/notif/new"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_particular_request(self, another_userID):
        url = self.url_base + "/users/" + str(self.userID) + "/notif/" + str(another_userID)
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_contacts(self):
        url = self.url_base + "/users/" + str(self.userID) + "/contacts"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_are_we_contacts(self, another_userID):
        url = self.url_base + "/users/" + str(self.userID) + "/contacts/" + str(another_userID)
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_number_new_messages(self):
        url = self.url_base + "/chat/" + str(self.userID) + "/new"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_id_last_message(self, another_userID):
        url = self.url_base + "/chat/" + str(self.userID) + "/" + str(another_userID) + "/last"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_including_messages(self, another_userID, params):
        url = self.url_base + "/chat/" + str(self.userID) + "/" + str(another_userID) + "/"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, params, headers)

    def get_popular(self):
        url = self.url_base + "/users/popular"
        headers = self.getAuthHeaderToken()
        return self.makeRequest_GET(url, "", headers)

    def get_user_recommended_to_another_user(self, another_userID):
        url = self.url_base + "/users/popular/" + str(self.userID) + "/" + str(another_userID)
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

    def get_job_positions(self):
        url = self.url_base + "/job_positions"
        return self.makeRequest_GET(url, "", {})

    def get_skills(self):
        url = self.url_base + "/skills"
        return self.makeRequest_GET(url, "", {})

    def get_job_position_by_category(self, category):
        url = self.url_base + "/job_positions/categories/" + category
        return self.makeRequest_GET(url, "", {})

    def get_job_position(self, job_position):
        url = self.url_base + "/job_positions/" + job_position
        return self.makeRequest_GET(url, "", {})

    def get_skills_by_category(self, category):
        url = self.url_base + "/skills/categories/" + category
        return self.makeRequest_GET(url, "", {})

    def get_skill(self, skill):
        url = self.url_base + "/skills/" + skill
        return self.makeRequest_GET(url, "", {})
