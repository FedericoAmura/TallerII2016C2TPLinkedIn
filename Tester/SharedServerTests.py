#!/usr/bin/python
import sys
import requests
import json

port = sys.argv[1]
headers = {'Content-Type': 'application/json'}

def printError(message, req):
	return message+", status: "+str(req)+", content: "+str(req.content)

print "---------- Checking empty data"
print "--------------- Empty categories"
req = requests.get("http://localhost:"+port+"/categories")
assert (req.content=='{"categories":[],"metadata":{"version":0.2,"count":0}}'),printError("Categories table not empty",req)
print "--------------- Empty jobs"
req = requests.get("http://localhost:"+port+"/job_positions")
assert (req.content=='{"job_positions":[],"metadata":{"version":0.2,"count":0}}'),printError("Job_positions table not empty",req)
print "--------------- Empty skills"
req = requests.get("http://localhost:"+port+"/skills")
assert (req.content=='{"skills":[],"metadata":{"version":0.2,"count":0}}'),printError("Skills table not empty",req)
print "---------- Empty data OK"



print "---------- Testing categories"

url = "http://localhost:"+port+"/categories"

print "--------------- Inserting Sonido"
payload = {"category":{"name":"Sonido","description":"Instrumentos musicales y eso"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"category":{"name":"Sonido","description":"Instrumentos musicales y eso"}}'),printError("Could not insert Sonido category",req)

print "--------------- Inserting Programar"
payload = {"category":{"name":"Programar","description":"El idioma de las computadoras"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"category":{"name":"Programar","description":"El idioma de las computadoras"}}'),printError("Could not insert Programar category",req)

print "--------------- Inserting Deportes"
payload = {"category":{"name":"Deportes","description":"Futbol y demases"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"category":{"name":"Deportes","description":"Futbol y demases"}}'),printError("Could not insert Deportes category",req)

print "--------------- Modifying Sonido"
url = "http://localhost:"+port+"/categories/Sonido"
payload = {"category":{"name":"Musica","description":"La voz del arte"}}
req = requests.put(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"category":{"name":"Musica","description":"La voz del arte"}}'),printError("Could not modify Sonido category",req)

print "--------------- Modifying Programar"
url = "http://localhost:"+port+"/categories/Programar"
payload = {"category":{"name":"Programacion","description":"El idioma de las computadoras"}}
req = requests.put(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"category":{"name":"Programacion","description":"El idioma de las computadoras"}}'),printError("Could not modify Programacion category",req)

print "--------------- Deleting Deportes"
url = "http://localhost:"+port+"/categories/Deportes"
req = requests.delete(url, headers=headers)
assert (req.status_code==204),"Could not delete Deportes category"

print "--------------- Checking final status"
url = "http://localhost:"+port+"/categories"
req = requests.get(url)
assert (req.content=='{"categories":[{"name":"Musica","description":"La voz del arte"},{"name":"Programacion","description":"El idioma de las computadoras"}],"metadata":{"version":0.2,"count":2}}'),printError("Categories table not correctly filled",req)

print "---------- Categories OK"



print "---------- Testing skills"

print "--------------- Inserting JavaEE"
url = "http://localhost:"+port+"/skills/categories/Programacion"
payload = {"skill":{"name":"JavaEE","description":"Enterprise Java development"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"skill":{"name":"JavaEE","description":"Enterprise Java development","category":"Programacion"}}'),printError("Could not insert JavaEE skill",req)

print "--------------- Inserting Python"
url = "http://localhost:"+port+"/skills/categories/Programacion"
payload = {"skill":{"name":"Python","description":"Robo-parcel"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"skill":{"name":"Python","description":"Robo-parcel","category":"Programacion"}}'),printError("Could not insert Python skill",req)

print "--------------- Inserting Guitarra"
url = "http://localhost:"+port+"/skills/categories/Musica"
payload = {"skill":{"name":"Guitarra","description":"Basico, pero muy fachero"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"skill":{"name":"Guitarra","description":"Basico, pero muy fachero","category":"Musica"}}'),printError("Could not insert Guitarra skill",req)

print "--------------- Inserting Futbol"
url = "http://localhost:"+port+"/skills/categories/Musica"
payload = {"skill":{"name":"Futbol","description":"Pelota redonda y mucha negrada"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"skill":{"name":"Futbol","description":"Pelota redonda y mucha negrada","category":"Musica"}}'),printError("Could not insert Futbol skill",req)

print "--------------- Modifying JavaEE"
url = "http://localhost:"+port+"/skills/categories/Programacion/JavaEE"
payload = {"skill":{"name":"Java","description":"All kinds of Java","category":"Programacion"}}
req = requests.put(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"skill":{"name":"Java","description":"All kinds of Java","category":"Programacion"}}'),printError("Could not modify JavaEE skill",req)

print "--------------- Modifying Futbol"
url = "http://localhost:"+port+"/skills/categories/Musica/Futbol"
payload = {"skill":{"name":"Soccer","description":"Futbol pero civilizado","category":"Musica"}}
req = requests.put(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"skill":{"name":"Soccer","description":"Futbol pero civilizado","category":"Musica"}}'),printError("Could not modify Futbol skill",req)

print "--------------- Deleting Soccer"
url = "http://localhost:"+port+"/skills/categories/Musica/Soccer"
req = requests.delete(url, headers=headers)
assert (req.status_code==204),"Could not delete Soccer skill"

print "--------------- Checking final status of Programacion category"
url = "http://localhost:"+port+"/skills/categories/Programacion"
req = requests.get(url)
assert (req.content=='{"skills":[{"name":"Python","description":"Robo-parcel","category":"Programacion"},{"name":"Java","description":"All kinds of Java","category":"Programacion"}],"metadata":{"version":0.2,"count":2}}'),printError("Skills table not correctly filled",req)

print "--------------- Checking final status"
url = "http://localhost:"+port+"/skills"
req = requests.get(url)
assert (req.content=='{"skills":[{"name":"Python","description":"Robo-parcel","category":"Programacion"},{"name":"Guitarra","description":"Basico, pero muy fachero","category":"Musica"},{"name":"Java","description":"All kinds of Java","category":"Programacion"}],"metadata":{"version":0.2,"count":3}}'),printError("Skills table not correctly filled",req)

print "---------- Skills OK"



print "---------- Testing jobs"

print "--------------- Inserting Java Developer"
url = "http://localhost:"+port+"/job_positions/categories/Programacion"
payload = {"job_position":{"name":"Java Developer","description":"A java writer"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"job_position":{"name":"Java Developer","description":"A java writer","category":"Programacion"}}'),printError("Could not insert Java Developer job",req)

print "--------------- Inserting Parcel scripter"
url = "http://localhost:"+port+"/job_positions/categories/Programacion"
payload = {"job_position":{"name":"Parcel scripter","description":"A guy from slytherin"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"job_position":{"name":"Parcel scripter","description":"A guy from slytherin","category":"Programacion"}}'),printError("Could not insert Parcel scripter job",req)

print "--------------- Inserting Guitarrista"
url = "http://localhost:"+port+"/job_positions/categories/Musica"
payload = {"job_position":{"name":"Guitarrista","description":"Se levanta todas"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"job_position":{"name":"Guitarrista","description":"Se levanta todas","category":"Musica"}}'),printError("Could not insert Guitarrista job",req)

print "--------------- Inserting Bajista"
url = "http://localhost:"+port+"/job_positions/categories/Musica"
payload = {"job_position":{"name":"Bajista","description":"Guitarra sin facha"}}
req = requests.post(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"job_position":{"name":"Bajista","description":"Guitarra sin facha","category":"Musica"}}'),printError("Could not insert Bajista job",req)

print "--------------- Modifying Parcel scripter"
url = "http://localhost:"+port+"/job_positions/categories/Programacion/Parcel%20scripter"
payload = {"job_position":{"name":"Python scripter","description":"Magic does not exist","category":"Programacion"}}
req = requests.put(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"job_position":{"name":"Python scripter","description":"Magic does not exist","category":"Programacion"}}'),printError("Could not modify Parcel scripter job",req)

print "--------------- Modifying Bajista"
url = "http://localhost:"+port+"/job_positions/categories/Musica/Bajista"
payload = {"job_position":{"name":"Cantante","description":"Otro fachero","category":"Musica"}}
req = requests.put(url, data=json.dumps(payload), headers=headers)
assert (req.content=='{"job_position":{"name":"Cantante","description":"Otro fachero","category":"Musica"}}'),printError("Could not modify Bajista job",req)

print "--------------- Deleting Cantante"
url = "http://localhost:"+port+"/job_positions/categories/Musica/Cantante"
req = requests.delete(url, headers=headers)
assert (req.status_code==204),"Could not delete Cantante job"

print "--------------- Checking final status of Programacion category"
url = "http://localhost:"+port+"/job_positions/categories/Programacion"
req = requests.get(url)
assert (req.content=='{"job_positions":[{"name":"Java Developer","description":"A java writer","category":"Programacion"},{"name":"Python scripter","description":"Magic does not exist","category":"Programacion"}],"metadata":{"version":0.2,"count":2}}'),printError("Jobs table not correctly filled",req)

print "--------------- Checking final status"
url = "http://localhost:"+port+"/job_positions"
req = requests.get(url)
assert (req.content=='{"job_positions":[{"name":"Java Developer","description":"A java writer","category":"Programacion"},{"name":"Guitarrista","description":"Se levanta todas","category":"Musica"},{"name":"Python scripter","description":"Magic does not exist","category":"Programacion"}],"metadata":{"version":0.2,"count":3}}'),printError("Jobs table not correctly filled",req)


print "---------- Jobs OK"


