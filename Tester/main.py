#!/usr/bin/python
import os
import psycopg2
import time
import unittest
import requests
import subprocess
import signal

print "----- Starting python master"

os.environ["CC"] = "gcc-4.9"
os.environ["CXX"] = "g++-4.9"

print "----- Creating database locally"
os.system('sudo ./Tester/initiatepostgres.sh')

print "----- Defining database environment variable for use in SharedServer"
os.environ["DATABASE_URL"] = "postgresql://postgres_user:password@127.0.0.1:5432/jobify_db"
os.environ["PORT"] = port = "5000"

#SharedServer
print "----- Running SharedServer (nodejs)"
os.system('nodejs ./SharedServer/index.js &')

print "Waiting for SharedServer to become active"
time.sleep(1)	#puede no ser necesario pero por las dudas lo esperamos 1 segundo

print "----- Running tests against SharedServer"
os.system("./Tester/SharedServerTests.py "+port)

#AppServer
print "----- Running AppServer (c++)"
proc_AppServer = subprocess.Popen(["./AppServer/bin/AppServer"], shell=False)

print "Waiting for AppServer to become active"
time.sleep(1)	#de nuevo, puede no ser necesario pero por las dudas lo esperamos 1 segundo


#print "----- Running tests against AppServer"
#os.system("./AppServer/UnitTest")


#ClientApp
print "----- Running ClientApp (android)"
os.system("python Tester/ClientTest.py")

print "------ Shutting Down the AppServer"
time.sleep(1)
os.kill(proc_AppServer.pid, signal.SIGINT)
#print "Waiting for ClientApp to become active"
#time.sleep(1)	#de nuevo, puede no ser necesario pero por las dudas lo esperamos 1 segundo

#print "----- Running tests against ClientApp"
#os.system("aca el comando para correr los test integradores")
#'''
