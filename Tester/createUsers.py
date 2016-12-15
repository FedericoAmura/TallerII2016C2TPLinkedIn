#!/usr/bin/env python
# -*- coding: utf-8 -*-
import base64
import hashlib
from Client import *

user1_data = {}
user2_data = {}
user3_data = {}
user4_data = {}
user5_data = {}
user6_data = {}

user1_data["first_name"] = "Emanuel"
user1_data["last_name"] = "Condo"
user1_data["birth"] =  "11/2/2000"
user1_data["email"] = "emanuelcondo@gmail.com"
user1_data["username"] = "emanuel"
user1_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user1_data["city"] = "CABA"
user1_data["longitude"] = 12.4
user1_data["latitude"] = 3.4

user2_data["first_name"] = "Guido"
user2_data["last_name"] = "Negri"
user2_data["birth"] =  "1/2/2000"
user2_data["email"] = "guidonegri@gmail.com"
user2_data["username"] = "guido"
user2_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user2_data["city"] = "CABA"
user2_data["longitude"] = 10.4
user2_data["latitude"] = 2.5

user3_data["first_name"] = "Federico"
user3_data["last_name"] = "Amura"
user3_data["birth"] =  "1/2/2000"
user3_data["email"] = "federicoamura@gmail.com"
user3_data["username"] = "federico"
user3_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user3_data["city"] = "CABA"
user3_data["longitude"] = 2.5
user3_data["latitude"] = 2.5

user4_data["first_name"] = "Francisco"
user4_data["last_name"] = "Marín"
user4_data["birth"] =  "1/2/2000"
user4_data["email"] = "franciscomarin@gmail.com"
user4_data["username"] = "francisco"
user4_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user4_data["city"] = "CABA"
user4_data["longitude"] = 2.5
user4_data["latitude"] = 2.5

user5_data["first_name"] = "Gabriel"
user5_data["last_name"] = "Fusca"
user5_data["birth"] =  "1/2/2000"
user5_data["email"] = "gabrielfusca@gmail.com"
user5_data["username"] = "gabriel"
user5_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user5_data["city"] = "CABA"
user5_data["longitude"] = 2.5
user5_data["latitude"] = 2.5

user6_data["first_name"] = "Chistian"
user6_data["last_name"] = "Calónico"
user6_data["birth"] =  "1/2/2000"
user6_data["email"] = "ccalonico@gmail.com"
user6_data["username"] = "chistian"
user6_data["password"] = base64.b64encode(hashlib.sha256("123456").digest())
user6_data["city"] = "CABA"
user6_data["longitude"] = 2.5
user6_data["latitude"] = 2.5

client1 = Client(user1_data)
client2 = Client(user2_data)
client3 = Client(user3_data)
client4 = Client(user4_data)
client5 = Client(user5_data)
client6 = Client(user6_data)

client1.signup()
client2.signup()
client3.signup()
client5.signup()
client6.signup()
