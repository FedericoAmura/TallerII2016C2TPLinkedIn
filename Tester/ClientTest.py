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
user3_data["longitude"] = 10.4
user3_data["latitude"] = 2.5

client1 = Client(user1_data)
client2 = Client(user2_data)
client3 = Client(user3_data)

client1.signup()
client2.signup()
client3.signup()

client1.login()
client2.login()

'''
    def setUp(self):
        res1 = client1.signup()
        res2 = client2.signup()
        self.assertEquals(201, res1.status_code)
        self.assertEquals(201, res2.status_code)

        res1 = client1.login()
        res2 = client2.login()
        self.assertEquals(200, res1.status_code)
        self.assertEquals(200, res2.status_code)

    def tearDown(self):
        res1 = client1.logout()
        res2 = client2.logout()
        self.assertEquals(204, res1.status_code)
        self.assertEquals(204, res2.status_code)
'''

class ClientTest(unittest.TestCase):

### POST
#    def test_login(self):
#        res1 = client1.login()
#        res2 = client2.login()
#        self.assertEquals(200, res1.status_code)
#        self.assertEquals(200, res2.status_code)

    # hacer un signup con un username ya existente debería devolver 422
    def test_signup_again(self):
        res = client1.signup()
        self.assertEquals(422, res.status_code)

    def test_accept_contact_request(self):
        another_userID = client2.get_user_id()
        res = client1.accept_contact_request(another_userID)
        self.assertEquals(204, res.status_code)

    def test_create_contact_request(self):
        data = {}
        data["targetID"] = client2.get_user_id()
        data["message"] = "Hello!!!"
        res = client1.create_contact_request(data)
        self.assertEquals(201, res.status_code)

    def test_notify_message_seen(self):
        data = {}
        data["targetID"] = client2.get_user_id()
        res = client1.notify_msg_seen(data)
        self.assertEquals(204, res.status_code)

    def test_send_message(self):
        data = {}
        data["receiverID"] = client2.get_user_id()
        data["message"] = "blabla"
        res = client1.send_msg(data)
        self.assertEquals(201, res.status_code)

### PUT
    def test_update_profile(self):
        data = {}
        data["name"] = "New Name"
        data["skills"] = ["skill1", "skill2"]
        res = client1.update_profile(data)
        self.assertEquals(200, res.status_code)

    def test_update_resume(self):
        data = {}
        data = {"resume" : "new resume"}
        res = client1.update_resume(data)
        self.assertEquals(204, res.status_code)

    def test_update_photo(self):
        data = {}
        data = {"photo": "asd12as"}
        res = client1.update_photo(data)
        self.assertEquals(204, res.status_code)

    def test_recommend_user(self):
        receiverID = client2.get_user_id()
        data = {}
        data["recommended"] = client3.get_user_id()
        data["receiver"] = receiverID
        data["recommends"] = True
        res = client1.recommend_user(receiverID, data)
        self.assertEquals(204, res.status_code)

### DELETE
    def test_logout(self):
        res = client3.login()
        self.assertEquals(200, res.status_code)
        res = client3.logout()
        self.assertEquals(204, res.status_code)

    def test_reject_contact_request(self):
        res = client1.reject_contact_request(client2.get_user_id())
        self.assertEquals(204, res.status_code)

    def test_delete_contact(self):
        res = client1.delete_contact(client2.get_user_id())
        self.assertEquals(204, res.status_code)

### GET
    def test_search_for_users(self):
        params = {} # query_string
        params["category"] = "cat"
        params["job_position"] = "job"
        res = client1.search_for_users(params)
        self.assertEquals(200, res.status_code)

    def test_get_profile_from_user(self):
        res = client1.get_profile_from(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_resume_from_user(self):
        res = client1.get_resume_from(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_photo_from_user(self):
        res = client1.get_photo_from(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_thumbnail_from_user(self):
        res = client1.get_photo_thumb_from(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_info_brief_from_user(self):
        res = client1.get_info_brief_from(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_pending_contact_request(self):
        res = client1.get_pending_contact_request()
        self.assertEquals(200, res.status_code)

    def test_get_number_pending_requests(self):
        res = client1.get_number_pending_requests()
        self.assertEquals(200, res.status_code)

    def test_get_particuliar_request(self):
        res = client1.get_particuliar_request(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_contacts(self):
        res = client1.get_contacts()
        self.assertEquals(200, res.status_code)

    def test_get_are_we_contacts(self):
        res = client1.get_are_we_contacts(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_number_new_messages(self):
        res = client1.get_number_new_messages()
        self.assertEquals(200, res.status_code)

    def test_get_id_last_msg_from_user(self):
        res = client1.get_id_last_message(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_including_messages(self):
        params = {"start":"12/2/2015", "end":"20/3/2015"}
        res = client1.get_including_messages(client2.get_user_id(), params)
        self.assertEquals(200, res.status_code)

    def test_get_popular(self):
        res = client1.get_popular()
        self.assertEquals(200, res.status_code)

    def test_if_user_recommended_to_another_user(self):
        res = client1.get_user_recommended_to_another_user(client2.get_user_id())
        self.assertEquals(200, res.status_code)

    def test_get_popular_by_position(self):
        res = client1.get_popular_by_position("pos")
        self.assertEquals(200, res.status_code)

    def test_get_popular_by_skill(self):
        res = client1.get_popular_by_skill("java")
        self.assertEquals(200, res.status_code)

    def test_get_categories(self):
        res = client1.get_categories()
        self.assertEquals(200, res.status_code)

    def test_get_job_position_by_category(self):
        res = client1.get_job_position_by_category("categ")
        self.assertEquals(200, res.status_code)

    def test_get_job_position(self):
        res = client1.get_job_position("pos")
        self.assertEquals(200, res.status_code)

    def test_get_skills_by_category(self):
        res = client1.get_skills_by_category("categ")
        self.assertEquals(200, res.status_code)

    def test_get_skill(self):
        res = client1.get_skill("java")
        self.assertEquals(200, res.status_code)


if __name__ == '__main__':
    unittest.main()

