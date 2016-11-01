#!/usr/bin/env python
# -*- coding: utf-8 -*-
import unittest
import base64
import hashlib
from Client import *

user1_data = {}
user2_data = {}
user3_data = {}

user1_data["first_name"] = "fulanito3"
user1_data["last_name"] = "perez"
user1_data["birth"] =  "11/2/2014"
user1_data["email"] = "mmmma@gmail.com"
user1_data["username"] = "guidonegri"
user1_data["password"] = base64.b64encode(hashlib.sha256("holaSoyFulano").digest())
user1_data["city"] = "ciudad_perdida"
user1_data["longitude"] = 12.4
user1_data["latitude"] = 3.4

user2_data["first_name"] = "menganito3"
user2_data["last_name"] = "perez"
user2_data["birth"] =  "1/2/2012"
user2_data["email"] = "rrrrra@gmail.com"
user2_data["username"] = "pepepipo"
user2_data["password"] = base64.b64encode(hashlib.sha256("holaSoyMengano").digest())
user2_data["city"] = "ciudad_perdida"
user2_data["longitude"] = 10.4
user2_data["latitude"] = 2.5

user3_data["first_name"] = "juan"
user3_data["last_name"] = "perez"
user3_data["birth"] =  "1/2/2012"
user3_data["email"] = "rrrrra@gmail.com"
user3_data["username"] = "juanperez"
user3_data["password"] = base64.b64encode(hashlib.sha256("holaSoyPerez").digest())
user3_data["city"] = "ciudad_perdida"
user3_data["longitude"] = "10.4"
user3_data["latitude"] = 2.5

user4_data = dict(user3_data);
del user4_data["username"]
#user4_data["city"] = None

client1 = Client(user1_data)
client2 = Client(user2_data)
client3 = Client(user3_data)
client4 = Client(user4_data)

class ClientTest(unittest.TestCase):

### POST
    #checked
    def test_00_login_before_signup(self):
        res = client1.login()
        self.assertEquals(403, res.status_code)

    #checked
    def test_01_signup(self):
        res1 = client1.signup()
        res2 = client2.signup()
        res3 = client3.signup()
        self.assertEquals(201, res1.status_code)
        self.assertEquals(201, res2.status_code)
        self.assertEquals(201, res3.status_code)

    # hacer un signup con datos inválidos (por ej, "city" con valor "None")
    #checked
    def test_02_signup_invalid_data(self):
        res = client4.signup()
        self.assertEquals(422, res.status_code)

    # hacer un signup con un username ya existente debería devolver 422
    #checked
    def test_03_repeated_signup(self):
        res = client1.signup()
        self.assertEquals(422, res.status_code)

    #checked
    def test_04_login(self):
        res1 = client1.login()
        res2 = client2.login()
        res3 = client3.login()
        data = json.loads(res1.text)
        self.assertTrue('token' in data)
        self.assertTrue('userID' in data)
        self.assertEquals(200, res1.status_code)
        self.assertEquals(200, res2.status_code)
        self.assertEquals(200, res3.status_code)

    #checked
    def test_05_accept_non_existent_contact_request(self):
        another_userID = "9999999"
        res = client1.accept_contact_request(another_userID)
        self.assertEquals(404, res.status_code)

    #checked
    def test_06_create_contact_request_to_non_existent_user(self):
        data = {}
        data["targetID"] = 999999999
        data["message"] = "Hello!!!"
        res = client1.create_contact_request(data)
        self.assertEquals(404, res.status_code)

    #checked
    def test_07_create_contact_request_to_existent_user(self):
        data = {}
        data["targetID"] = client2.get_user_id()
        data["message"] = "Hello!!!"
        res1 = client1.create_contact_request(data)
        data["targetID"] = client3.get_user_id()
        res2 = client1.create_contact_request(data)
        self.assertEquals(201, res1.status_code)
        self.assertEquals(201, res2.status_code)

    #checked
    def test_08_accept_existent_contact_request(self):
        another_userID = client1.get_user_id()
        res = client2.accept_contact_request(another_userID)
        self.assertEquals(204, res.status_code)

    #NOT checked (TODO falta implementación en DBJSON (debería ser 400))
    def test_09_send_message_to_non_existent_user(self):
        data = {}
        data["receiverID"] = 999999
        data["message"] = "blabla"
        res = client1.send_msg(data)
        self.assertEquals(201, res.status_code)

    #NOT checked (TODO falta implementación en DBJSON)
    def test_10_send_message_to_existent_user(self):
        data = {}
        data["receiverID"] = client2.get_user_id()
        data["message"] = "blabla"
        res = client1.send_msg(data)
        self.assertEquals(201, res.status_code)

    #NOT checked (TODO falta implementación en DBJSON (debería ser 400))
    def test_11_notify_message_seen_to_non_existent_chat(self):
        data = {}
        data["targetID"] = "999999"
        res = client1.notify_msg_seen(data)
        self.assertEquals(204, res.status_code)

    #NOT checked (TODO falta implementación en DBJSON)
    def test_12_notify_message_seen_to_existent_chat(self):
        data = {}
        data["targetID"] = client1.get_user_id()
        res = client2.notify_msg_seen(data)
        self.assertEquals(204, res.status_code)

### PUT
    #checked
    def test_13_update_profile(self):
        data = {}
        data["name"] = "New Name"
        data["skills"] = ["skill1", "skill2"]
        res = client1.update_profile(data)
        #TODO sólo debería enviar los datos que quiero actualizar
        # esto devuelve un error porque no están todos los campos completos
        self.assertEquals(422, res.status_code)

    #checked
    def test_14_update_resume(self):
        data = {}
        data = {"resume" : "new resume"}
        res = client1.update_resume(data)
        self.assertEquals(204, res.status_code)

    #checked
    def test_15_update_photo(self):
        data = {}
        data = {"photo": "asd12as"}
        res = client1.update_photo(data)
        self.assertEquals(204, res.status_code)

    #NOT checked (TODO falta implementación en DBJSON)
    def test_16_recommend_user(self):
        data = {}
        data["recommended"] = int(client3.get_user_id())
        data["recommends"] = True
        res = client1.recommend_user(data)
        self.assertEquals(204, res.status_code)

### DELETE
    #checked
    def test_17_reject_contact_request(self):
        res = client3.reject_contact_request(client1.get_user_id())
        self.assertEquals(204, res.status_code)

    #checked
    def test_18_delete_nonexistent_contact(self):
        res = client1.delete_contact("99999")
        self.assertEquals(404, res.status_code)

    #checked
    def test_19_delete_existent_contact(self):
        another_userID = client2.get_user_id()
        res = client1.delete_contact(another_userID)
        self.assertEquals(204, res.status_code)

    #checked
    def test_20_logout(self):
        res = client3.logout()
        self.assertEquals(204, res.status_code)
        res = client3.get_contacts()
        self.assertEquals(403, res.status_code)

### GET
    #NOT checked (TODO falta implementación en DBJSON y AppServer)
    def test_21_search_for_users(self):
        params = {} # query_string
        params["category"] = "cat"
        params["job_position"] = "job"
        res = client1.search_for_users(params)
        self.assertEquals(200, res.status_code)

    #checked
    def test_22_get_profile_from_user(self):
        res = client1.get_profile_from(client2.get_user_id())
        data = json.loads(res.text)
        self.assertTrue('name' in data)
        self.assertTrue('city' in data)
        self.assertEquals(200, res.status_code)

    #checked
    def test_23_get_resume_from_user(self):
        res = client1.get_resume_from(client2.get_user_id())
        data = json.loads(res.text)
        self.assertTrue('resume' in data)
        self.assertEquals(200, res.status_code)

    #checked
    def test_24_get_photo_from_user(self):
        res = client1.get_photo_from(client2.get_user_id())
        data = json.loads(res.text)
        self.assertTrue('photo' in data)
        self.assertEquals(200, res.status_code)

    #checked
    def test_25_get_thumbnail_from_user(self):
        res = client1.get_photo_thumb_from(client2.get_user_id())
        data = json.loads(res.text)
        self.assertTrue('thumb' in data)
        self.assertEquals(200, res.status_code)

    #checked
    def test_26_get_info_brief_from_user(self):
        res = client1.get_info_brief_from(client2.get_user_id())
        data = json.loads(res.text)
        self.assertTrue('name' in data)
        self.assertTrue('thumb' in data)
        self.assertEquals(200, res.status_code)

    #checked
    def test_27_get_pending_contact_request(self):
        res = client1.get_pending_contact_request()
        data = json.loads(res.text)
        self.assertTrue('pending' in data)
        self.assertEquals(200, res.status_code)

    #checked
    def test_28_get_contacts_get_number_pending_requests(self):
        res = client1.get_number_pending_requests()
        data = json.loads(res.text)
        self.assertTrue('count' in data)
        self.assertEquals(200, res.status_code)

    #NOT checked (TODO ?)
    def test_29_get_particular_request(self):
        res = client1.get_particular_request(client2.get_user_id())
        self.assertEquals(404, res.status_code)

    #NOT checked (TODO falta implementación de DBJSON)
    def test_30_get_contacts(self):
        res = client1.get_contacts()
        self.assertEquals(200, res.status_code)

    #checked
    def test_31_get_are_we_contacts(self):
        res = client1.get_are_we_contacts(client2.get_user_id())
        self.assertEquals(404, res.status_code)

    def test_32_get_number_new_messages(self):
        res = client1.get_number_new_messages()
        self.assertEquals(200, res.status_code)

    def test_33_get_id_last_msg_from_user(self):
        res = client1.get_id_last_message(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_34_get_including_messages(self):
        params = {"start":"12/2/2015", "end":"20/3/2015"}
        res = client1.get_including_messages(client2.get_user_id(), params)
        self.assertEquals(200, res.status_code)

    def test_35_get_popular(self):
        res = client1.get_popular()
        self.assertEquals(200, res.status_code)

    def test_36_if_user_recommended_to_another_user(self):
        res = client1.get_user_recommended_to_another_user(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_37_get_popular_by_position(self):
        res = client1.get_popular_by_position("pos")
        self.assertEquals(200, res.status_code)

    def test_38_get_popular_by_skill(self):
        res = client1.get_popular_by_skill("java")
        self.assertEquals(200, res.status_code)

    #checked
    def test_39_get_categories(self):
        res = client1.get_categories()
        self.assertEquals(200, res.status_code)

    #checked
    def test_40_get_job_position_by_nonexistent_category(self):
        res = client1.get_job_position_by_category("nonexistent")
        self.assertEquals(404, res.status_code)

    #checked
    def test_41_get_nonexistent_job_position_get_job_position_by_existent_category(self):
        res = client1.get_job_position_by_category("Musica")
        self.assertEquals(200, res.status_code)

    #checked
    def test_42_get_nonexistent_job_position(self):
        res = client1.get_job_position("nonexistent")
        self.assertEquals(404, res.status_code)

    #checked
    def test_43_get_existent_job_position(self):
        res = client1.get_job_position("Tester")
        self.assertEquals(200, res.status_code)

    #checked
    def test_44_get_skills_by_nonexistent_category(self):
        res = client1.get_skills_by_category("nonexistent")
        self.assertEquals(404, res.status_code)

    #checked
    def test_45_get_skills_by_existent_category(self):
        res = client1.get_skills_by_category("Programacion")
        self.assertEquals(200, res.status_code)

    #checked
    def test_46_get_nonexistent_skill(self):
        res = client1.get_skill("nonexistent")
        self.assertEquals(404, res.status_code)

    #checked
    def test_47_get_existent_skill(self):
        res = client1.get_skill("Python")
        self.assertEquals(200, res.status_code)


if __name__ == '__main__':
    unittest.main()
