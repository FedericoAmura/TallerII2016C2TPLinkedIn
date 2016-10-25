#!/usr/bin/env python
# -*- coding: utf-8 -*-
import unittest
from Client import *

user1_data = {}
user2_data = {}

user1_data["first_name"] = "fulanito"
user1_data["last_name"] = "perez"
user1_data["birth"] =  "11/2/2014"
user1_data["email"] = "mmmm@gmail.com"
user1_data["username"] = "cosmefulanito"
user1_data["password"] = "mi_password"
user1_data["city"] = "ciudad_perdida"
user1_data["longitude"] = 12.4
user1_data["latitude"] = 3.4

user2_data["first_name"] = "menganito"
user2_data["last_name"] = "perez"
user2_data["birth"] =  "1/2/2012"
user2_data["email"] = "rrrrr@gmail.com"
user2_data["username"] = "cosmemenganito"
user2_data["password"] = "mi_password"
user2_data["city"] = "ciudad_perdida"
user2_data["longitude"] = 10.4
user2_data["latitude"] = 2.5

client1 = Client(user1_data)
client2 = Client(user2_data)

class ClientTest(unittest.TestCase):
### POST
    def test_login(self):
        res = client1.login()
        self.assertNotEquals(200, res.status_code)

    def test_signup(self):
        res = client1.signup()
        self.assertEquals(201, res.status_code)

    def test_accept_contact_request(self):
        other_userID = "123"
        res = client1.accept_contact_request(other_userID)
        self.assertEquals(204, res.status_code)

    def test_create_contact_request(self):
        data = {}
        data.clear()
        data["receiverID"] = "123"
        data["message"] = "Hello!!!"
        res = client1.create_contact_request(data)
        self.assertEquals(201, res.status_code)

    def test_notify_message_seen(self):
        data = {}
        data.clear()
        data["receiverID"] = "123"
        res = client1.notify_msg_seen(data)
        self.assertEquals(204, res.status_code)

    def test_send_message(self):
        data = {}
        data.clear()
        data["targetID"] = "123"
        data["message"] = "blabla"
        res = client1.send_msg(data)
        self.assertEquals(201, res.status_code)

### PUT
    def test_update_profile(self):
        data = {}
        data.clear()
        data["name"] = "New Name"
        data["skills"] = ["skill1", "skill2"]
#        print str(data)
        res = client1.update_profile(data)
        self.assertEquals(200, res.status_code)

    def test_update_summary(self):
        data = {}
        data.clear()
        data = {"summary" : "new summary"}
        res = client1.update_summary(data)
        self.assertEquals(204, res.status_code)

    def test_update_photo(self):
        data = {}
        data.clear()
        data = {"photo": "asd12as"}
        res = client1.update_photo(data)
        self.assertEquals(204, res.status_code)

    def test_recommend_user(self):
        receiverID = "123"
        data = {}
        data.clear()
        data["recommended"] = "456"
        data["recommends"] = True
        res = client1.recommend_user(receiverID, data)
        self.assertEquals(204, res.status_code)

### DELETE
    def test_logout(self):
        res = client1.close_session()
        self.assertEquals(204, res.status_code)

    def test_reject_contact_request(self):
        res = client1.reject_contact_request("456")
        self.assertEquals(204, res.status_code)

    def test_delete_contact(self):
        res = client1.delete_contact("123")
        self.assertEquals(204, res.status_code)

### GET
    def test_search_for_users(self):
        params = {} # query_string
        params["category"] = "cat"
        params["job_position"] = "job"
        res = client1.search_for_users(params)
        self.assertEquals(200, res.status_code)

    def test_get_profile_from_user(self):
        res = client1.get_profile_from("123")
        self.assertEquals(200, res.status_code)

    def test_get_summary_from_user(self):
        res = client1.get_summary_from("123")
        self.assertEquals(200, res.status_code)

    def test_get_photo_from_user(self):
        res = client1.get_photo_from("123")
        self.assertEquals(200, res.status_code)

    def test_get_thumbnail_from_user(self):
        res = client1.get_photo_thumb_from("123")
        self.assertEquals(200, res.status_code)

    def test_get_info_brief_from_user(self):
        res = client1.get_info_brief_from("123")
        self.assertEquals(200, res.status_code)

    def test_get_pending_contact_request(self):
        res = client1.get_pending_contact_request()
        self.assertEquals(200, res.status_code)

    def test_get_number_pending_requests(self):
        res = client1.get_number_pending_requests()
        self.assertEquals(200, res.status_code)

    def test_get_particuliar_request(self):
        res = client1.get_particuliar_request("123")
        self.assertEquals(200, res.status_code)

    def test_get_contacts(self):
        res = client1.get_contacts()
        self.assertEquals(200, res.status_code)

    def test_get_are_we_contacts(self):
        res = client1.get_are_we_contacts("123")
        self.assertEquals(200, res.status_code)

    def test_get_number_new_messages(self):
        res = client1.get_number_new_messages()
        self.assertEquals(200, res.status_code)

    def test_get_id_last_msg_from_user(self):
        res = client1.get_id_last_message("123")
        self.assertEquals(200, res.status_code)

    def test_get_including_messages(self):
        params = {"start":"12/2/2015", "end":"20/3/2015"}
        res = client1.get_including_messages("123", params)
        self.assertEquals(200, res.status_code)

    def test_get_popular(self):
        res = client1.get_popular()
        self.assertEquals(200, res.status_code)

    def test_if_user_recommended_to_another_user(self):
        res = client1.get_user_recommended_to_another_user("123")
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
