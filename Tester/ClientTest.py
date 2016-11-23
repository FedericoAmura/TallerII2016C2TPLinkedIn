#!/usr/bin/env python
# -*- coding: utf-8 -*-
import unittest
import base64
import hashlib
from Client import *

user1_data = {}
user2_data = {}
user3_data = {}

user1_data["first_name"] = "Emanuel"
user1_data["last_name"] = "Condo"
user1_data["birth"] =  "11/2/2014"
user1_data["email"] = "emanuelcondo@gmail.com"
user1_data["username"] = "emanuel"
user1_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user1_data["city"] = "ciudad_perdida"
user1_data["longitude"] = 12.4
user1_data["latitude"] = 3.4

user2_data["first_name"] = "Guido"
user2_data["last_name"] = "Negri"
user2_data["birth"] =  "1/2/2012"
user2_data["email"] = "guidonegri@gmail.com"
user2_data["username"] = "guido"
user2_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user2_data["city"] = "ciudad_perdida"
user2_data["longitude"] = 10.4
user2_data["latitude"] = 2.5

user3_data["first_name"] = "Federico"
user3_data["last_name"] = "Amura"
user3_data["birth"] =  "1/2/2012"
user3_data["email"] = "federicoamura@gmail.com"
user3_data["username"] = "fede"
user3_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user3_data["city"] = "ciudad_perdida"
user3_data["longitude"] = "10.4"
user3_data["latitude"] = 2.5

user4_data = dict(user3_data);
del user4_data["username"]
#user4_data["city"] = None

user5_data = dict(user3_data)
user5_data["first_name"] = "Lionel"
user5_data["last_name"] = "Messi"
user5_data["username"] = "lionelmessi"
user5_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())

user6_data = dict(user5_data)
user6_data["username"] = "francisco"
user6_data["first_name"] = "Francisco"
user6_data["last_name"] = "Marin"
user6_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())

client1 = Client(user1_data)
client2 = Client(user2_data)
client3 = Client(user3_data)
client4 = Client(user4_data)
client5 = Client(user5_data)
client6 = Client(user6_data)

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
        res5 = client5.signup()
        res6 = client6.signup()
        self.assertEquals(201, res1.status_code)
        self.assertEquals(201, res2.status_code)
        self.assertEquals(201, res3.status_code)
        self.assertEquals(201, res5.status_code)
        self.assertEquals(201, res6.status_code)

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
        res5 = client5.login()
        res6 = client6.login()
        data = json.loads(res1.text)
        self.assertTrue('token' in data)
        self.assertTrue('userID' in data)
        self.assertEquals(200, res1.status_code)
        self.assertEquals(200, res2.status_code)
        self.assertEquals(200, res3.status_code)
        self.assertEquals(200, res5.status_code)
        self.assertEquals(200, res6.status_code)

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
        data["targetID"] = client5.get_user_id()
        res5 = client1.create_contact_request(data)
        data["targetID"] = client6.get_user_id()
        res6 = client1.create_contact_request(data)
        self.assertEquals(200, res1.status_code)
        self.assertEquals(200, res2.status_code)
        self.assertEquals(200, res5.status_code)
        self.assertEquals(200, res6.status_code)

    #checked
    def test_08_accept_existent_contact_request(self):
        another_userID = client1.get_user_id()
        res = client2.accept_contact_request(another_userID)
        self.assertEquals(204, res.status_code)
        res5 = client5.accept_contact_request(another_userID)
        self.assertEquals(204, res5.status_code)
        res6 = client6.accept_contact_request(another_userID)
        self.assertEquals(204, res6.status_code)

    #checked
    def test_09_send_message_to_non_existent_user(self):
        data = {}
        data["receiverID"] = 999999
        data["message"] = "blabla"
        res = client1.send_msg(data)
        self.assertEquals(404, res.status_code)

    #checked
    def test_10_send_message_to_existent_user(self):
        data = {}
        data["receiverID"] = client2.get_user_id()
        data["message"] = "blabla"
        res = client1.send_msg(data)
        self.assertEquals(201, res.status_code)
        res2 = client2.get_number_new_messages()
        data = json.loads(res2.text)
        self.assertEquals(200, res2.status_code)
        self.assertEquals(len(data["new"]), 1)
        self.assertEquals(data["new"][0]["senderID"], client1.get_user_id())

    #checked
    def test_11_notify_message_seen_to_non_existent_chat(self):
        data = {}
        data["targetID"] = 999999
        res = client1.notify_msg_seen(data)
        self.assertEquals(404, res.status_code)

    #checked
    def test_12_notify_message_seen_to_existent_chat(self):
        data = {}
        data["targetID"] = client1.get_user_id()
        res = client2.notify_msg_seen(data)
        self.assertEquals(204, res.status_code)

### PUT
    #checked
    def test_13_update_profile(self):
        data = {}
        data["name"] = "Emanuel Condo"
        data["birth"] = "12/12/1900"
        data["email"] = "emanuelcondo@gmail.com"
        data["city"] = "ciudadBs"
        data["job_positions"] = []
        data["skills"] = ["Android", "Python"]
        data["latitude"] = 1.2
        data["longitude"] = 2.2
        res = client1.update_profile(data)
        self.assertEquals(204, res.status_code)

        data = {}
        data["name"] = "Francisco Marin"
        data["birth"] = "12/12/1900"
        data["email"] = "francisco@gmail.com"
        data["city"] = "ciudadBs"
        data["job_positions"] = [{"name":"Android developer", "start":"12/12/2010", "end":"13/12/2010"},
                                 {"name":"Tester", "start":"20/2/2011", "end":"current"}]
        data["skills"] = ["Android"]
        data["latitude"] = 1.2
        data["longitude"] = 2.2
        res = client6.update_profile(data)
        self.assertEquals(204, res.status_code)

    #checked
    def test_14_update_resume(self):
        data = {}
        data = {"resume" : "new resume"}
        res = client1.update_resume(data)
        self.assertEquals(204, res.status_code)

    #checked
    def test_15_update_photo(self):
        data = {}
        data = {"photo": "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"}
        res = client1.update_photo(data)
        self.assertEquals(204, res.status_code)

    #checked
    def test_16_recommend_user(self):
        data = {}
        data["recommended"] = client3.get_user_id()
        data["recommends"] = True
        res = client1.recommend_user(data)
        self.assertEquals(204, res.status_code)
        res = client1.get_profile_from(client3.get_user_id())
        data = json.loads(res.text)
        self.assertEquals(data['popularidad'], 1)

    #checked
    def test_16_recommend_nonexistent_user(self):
        data = {}
        data["recommended"] = 10
        data["recommends"] = True
        res = client1.recommend_user(data)
        self.assertEquals(404, res.status_code)

### DELETE
    #checked
    def test_17_reject_contact_request(self):
        res = client3.reject_contact_request(client1.get_user_id())
        self.assertEquals(200, res.status_code)

    #checked
    def test_18_delete_nonexistent_contact(self):
        res = client1.delete_contact(99999)
        self.assertEquals(404, res.status_code)

    #checked
    def test_19_delete_existent_contact(self):
        another_userID = client2.get_user_id()
        res = client1.delete_contact(another_userID)
        self.assertEquals(200, res.status_code)

    #checked
    def test_20_logout(self):
        res = client3.logout()
        self.assertEquals(200, res.status_code)
        res = client3.get_contacts()
        self.assertEquals(403, res.status_code)

### GET
    #checked
    def test_21_search_for_users(self):
        params = {} # query_string
        params["job_position"] = "job1;job2"
        params["skill"] = "skill1;skill2"
        params["distance"] = 2.4
        params["geolocation"] = "2.5;5.8"
        params["popsort"] = True
        res = client1.search_for_users(params)
        self.assertEquals(400, res.status_code)
        params.clear()
        params["job_position"] = "Tester"
        params["skill"] = "Android"
        res = client1.search_for_users(params)
        self.assertEquals(200, res.status_code)
        data = json.loads(res.text)
        print res.text
        self.assertEquals(len(data['users']), 2)

    #checked
    def test_210_search_all_users(self):
        res = client1.search_for_users({})
        data = json.loads(res.text)
        self.assertEquals(200, res.status_code)
        self.assertEquals(len(data['users']), 5)

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

    #checked
    def test_29_get_particular_request(self):
        res = client1.get_particular_request(client2.get_user_id())
        self.assertEquals(404, res.status_code)
        data = {}
        data["targetID"] = client2.get_user_id()
        data["message"] = "quiero ser tu amigo"
        res = client1.create_contact_request(data)
        self.assertEquals(200, res.status_code)
        res = client2.get_particular_request(client1.get_user_id())
        self.assertEquals(200, res.status_code)
        data = json.loads(res.text)
        self.assertEquals(client1.get_user_id(), data["userID"])
        self.assertEquals(data["message"], "quiero ser tu amigo")

    #checked
    def test_30_get_contacts(self):
        res = client1.get_contacts()
        self.assertEquals(200, res.status_code)
        data = json.loads(res.text)
        self.assertEquals(len(data["contacts"]),2)

    #checked
    def test_31_get_are_we_contacts(self):
        res = client1.get_are_we_contacts(client2.get_user_id())
        self.assertEquals(404, res.status_code)

    #checked
    def test_32_get_number_new_messages(self):
        res = client1.get_number_new_messages()
        self.assertEquals(200, res.status_code)

    #checked
    def test_33_get_id_last_msg_from_user(self):
        res = client1.get_id_last_message(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_34_get_including_messages(self):
        params = {"start":1, "end":2}
        res = client1.get_including_messages(client2.get_user_id(), params)
        self.assertEquals(200, res.status_code)

    #checked
    def test_35_if_user_recommended_to_another_user(self):
        res = client1.get_user_recommended_to_another_user(client2.get_user_id())
        self.assertEquals(200, res.status_code)
        data = json.loads(res.text)
        self.assertFalse(data["recommends"])
        res = client1.get_user_recommended_to_another_user(client3.get_user_id())
        self.assertEquals(200, res.status_code)
        data = json.loads(res.text)
        self.assertTrue(data["recommends"])

    #checked
    def test_36_get_popular(self):
        res = client1.get_popular()
        data = json.loads(res.text)
        self.assertTrue('users' in data)
        self.assertNotEquals(len(data['users']), 0)
        self.assertEquals(200, res.status_code)

    #checked
    def test_37_get_popular_by_position(self):
        res = client1.get_popular_by_position("pos")
        data = json.loads(res.text)
        self.assertTrue('users' in data)
        self.assertTrue('position' in data)
        self.assertEquals(len(data['users']),0)
        self.assertEquals(200, res.status_code)

    #checked
    def test_38_get_popular_by_skill(self):
        res = client1.get_popular_by_skill("Android")
        data = json.loads(res.text)
        self.assertTrue('users' in data)
        self.assertTrue('skill' in data)
        self.assertNotEquals(len(data['users']), 0)
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

    def test_48_get_job_positions(self):
        res = client1.get_job_positions()
        self.assertEquals(200, res.status_code)

    def test_49_get_skills(self):
        res = client1.get_skills()
        self.assertEquals(200, res.status_code)


if __name__ == '__main__':
    unittest.main()
