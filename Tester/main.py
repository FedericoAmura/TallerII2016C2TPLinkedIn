#!/usr/bin/python
import os
import psycopg2
import time
import unittest
import requests
print "----- Starting python master"

print "----- Creating database"
os.system('./Tester/initiatepostgres.sh')

print "----- Connecting to dabase"
conn = psycopg2.connect(database="jobify_db", user="postgres_user", password="password", host="127.0.0.1", port="5432")
print "----- Opened database successfully"

cur = conn.cursor()
print "----- Inserting test categories..."
cur.execute("INSERT INTO categories (name, description) VALUES ('Musica', 'Instrumentos musicales y eso'),('Programacion', 'El idioma de las computadoras'),('Cocina', 'Tengo hambre'),('Abogacia', 'Te imaginas un mundo sin Abogacia?')")

print "----- Inserting test skills..."
cur.execute("INSERT INTO skills (name, description, category) VALUES ('Python', 'Lenguaje de las serpientesitas','Programacion'),('Android', 'El SO de los celulares de hoy','Programacion'),('Freir churros', 'Para laburar en el topo','Cocina'),('Reposteria', 'Que ganas de torta','Cocina'),('Flauta', 'Instrumento largo que se sopla','Musica')")

print "----- Inserting test jobs..."
cur.execute("INSERT INTO jobs (name, description, category) VALUES ('Flautista', 'Conocido tambien como el soplaquena','Musica'),('Android developer', 'El que hace apps','Programacion'),('Chef', 'Muy probablemente tambien sea gordo','Cocina'),('Tester', 'Nadie los quiere, pero te salvan las papas','Programacion'),('Baterista', 'Siempre pone casa para tocar','Musica'),('Abogado', 'Uno te mete, otro te saca','Abogacia')")

conn.commit()
print "----- Test data inserted succesfully";
conn.close()

print "Defining database environment variable for nodejs"
os.environ["DATABASE_URL"] = "postgresql://postgres_user:password@127.0.0.1:5432/jobify_db"
os.environ["PORT"] = port = "5000"

print "Running nodejs"
os.system('nodejs ./SharedServer/index.js &')

time.sleep(1)

print "Running tests agains SharedServer"
req = requests.get("http://localhost:"+port+"/job_positions")
print req.content


'''
def fun(x):
    return x+1

class TestRunner(unittest.TestCase):
    #cada funcion es un test
    def prueba(self):
        self.assertEqual(fun(3), 4, "Test Prueba");
'''