#include "../include/gtest/gtest.h"
#include "../include/database/DBJSON.h"
#include "../include/database/DBExceptions.h"
#include "../include/database/DBConstants.h"
#include "../include/database/JsonChecker.h"
#include <iostream>
#include <cstdio>

using json11::Json;

/**
 * Tests sobre la clase DBJSON
 */

static const string defaultFecha("22/2/1922");
static const string defaultNombre("TNombre");
static const string defaultApellido("TApellido");
static const string defaultEmail("email@dominio.com");
static const string defaultCiudad("Ciudad");
static const string defaultBase64PassHash("YWJjZGVmZ2hpamtsbW5vcHFyc3R1dncwMTIzNDU2Nzg=");
static const string dbPath("/tmp/testjsondb");
static const std::string tinyJPGBase64 = "/9j/4AAQSkZJRgABAQEASABIA"
		"AD/2wBDAP/////////////////////////////////////////////////"
		"/////////////////////////////////////wgALCAABAAEBAREA/8QAF"
		"BABAAAAAAAAAAAAAAAAAAAAAP/aAAgBAQABPxA=";

class DBJsonTest : public ::testing::Test {
 public:
	DBJSON *dbj;
	DBJsonTest()
	{
		leveldb::Status status = leveldb::DestroyDB(dbPath, leveldb::Options());
		if(!status.ok()) std::cout << "Error elminando DB: " << status.ToString() << std::endl;
		DBRaw *db = new DBRaw(dbPath, &std::cout);
		dbj = new DBJSON(db);
	}
	~DBJsonTest() {
		delete dbj;
	}

	/**
	 * Registra un usuario con varios parametros por defecto
	 * @param userName				Nombre de usuario
	 * @param latitud				Latitud
	 * @param longitud				Longitud
	 * @return						El hash de la password
	 */
	uint32_t registrarTest(string userName, double latitud, double longitud) {
		Json json = Json::object {
			{ "first_name", defaultNombre },
			{ "last_name", defaultApellido },
			{ "birth", defaultFecha },
			{ "email", defaultEmail },
			{ "username", userName },
			{ "password", defaultBase64PassHash },
			{ "city", defaultCiudad },
			{ "longitude", longitud },
			{ "latitude", latitud },
		};
		return dbj->registrarse(json);
	}
	void registrarN(int n, int offset = 0){
		for (int i = offset; i < n+offset; ++i)
		{
			string username = string("Username").append(std::to_string(i));
			registrarTest(username, (double)n, (double)n);
		}
	}
};

TEST_F(DBJsonTest, testRegistroYLogin)
{
	string userName1("TUsername1");
	string userName2("TUsername2");
	EXPECT_EQ(registrarTest(userName1, 1.0, 0.5),0);
	EXPECT_EQ(registrarTest(userName2, 0.4, 0.3),1);
	EXPECT_THROW(registrarTest(userName2, 0.4, 0.3),PreexistentUsername);
	Json loginJson1 = Json::object {
				{ "username", userName1 },
				{ "password", defaultBase64PassHash },
	};
	EXPECT_EQ(dbj->login(loginJson1),0);
	Json loginJson2 = Json::object {
				{ "username", userName2 },
				{ "password", defaultBase64PassHash },
	};
	EXPECT_EQ(dbj->login(loginJson2),1);
	Json loginJson3 = Json::object {
				{ "username", userName2 },
				{ "password", "asd" },
	};
	EXPECT_THROW(dbj->login(loginJson3),BadPasswordSize);
	string casiPass(defaultBase64PassHash);
	casiPass[0] = 'E';
	Json loginJson4 = Json::object {
				{ "username", userName2 },
				{ "password", casiPass },
	};
	EXPECT_THROW(dbj->login(loginJson4),BadPassword);
}

TEST_F(DBJsonTest, testGetSetResumen)
{
	string userName("Username");
	uint32_t uid = registrarTest(userName, 1.0, 0.5);
	Json resumenJson = Json::object {
		{ "resume", "Test resumen." }
	};
	dbj->setResumen(uid, resumenJson);
	Json result(dbj->getResumen(uid));
	EXPECT_STREQ(result["resume"].string_value().c_str(), "Test resumen.");
}

TEST_F(DBJsonTest, testGetSetFoto)
{
	string userName("Username");
	uint32_t uid = registrarTest(userName, 1.0, 0.5);
	Json fotoJson = Json::object {
		{ "photo", tinyJPGBase64 }
	};
	dbj->setFoto(uid, fotoJson);
	Json resultFoto(dbj->getFoto(uid));
	Json resultThumb(dbj->getFotoThumbnail(uid));
	EXPECT_STREQ(tinyJPGBase64.c_str(), resultFoto["photo"].string_value().c_str());
	EXPECT_GT(resultThumb["thumb"].string_value().length(), 0);
}

TEST_F(DBJsonTest, testGetSetPerfil)
{
	string userName("Username");
	uint32_t uid = registrarTest(userName, 1.0, 0.5);

	// Prueba can escases de datos
	Json datosJson1 = Json::object {
		{ "name" , "Nombre Test" },
		{ "birth", "4/4/1994" },
		{ "email", "mail1@test.com" },
		{ "skills", Json::array { } },
		{ "job_positions",  Json::array { } },
		{ "longitude", 1.0},
		{ "latitude", 0.5},
		{ "city" , "Una ciudad" },
	};
	dbj->setDatos(uid, datosJson1);
	Json result1(dbj->getDatos(uid));

	EXPECT_STREQ(result1["name"].string_value().c_str(), "Nombre Test");
	EXPECT_STREQ(result1["email"].string_value().c_str(), "mail1@test.com");
	EXPECT_STREQ(result1["birth"].string_value().c_str(), "4/4/1994");
	EXPECT_LT(tinyJPGBase64.length(), result1["photo"].string_value().length());
	EXPECT_STREQ(result1["resume"].string_value().c_str(), "");
	EXPECT_EQ(0, result1["contacts"].int_value());
	EXPECT_EQ(0, result1["popularidad"].int_value());
	EXPECT_NEAR(result1["longitude"].number_value(), 1, 0.0001);
	EXPECT_NEAR(result1["latitude"].number_value(), 0.5, 0.0001);
	EXPECT_TRUE(result1["skills"].array_items().empty());
	EXPECT_TRUE(result1["puestos"].array_items().empty());

	// Prueba con abundancia de datos
	Json datosJson2 = Json::object {
		{ "name", "Nombre Test" },
		{ "email", "nuevo@mail.com"},
		{ "birth", "15/5/1932"},
		{ "skills", Json::array { "Skill1", "Skill2" } },
		{"job_positions",  Json::array { Json::object {
			{ "name", "Puesto1"},
			{ "start", "4/11/1994"},
			{ "end", "current"}
			},
			Json::object {
			{ "name", "Puesto2"},
			{ "start", "4/2/1990"},
			{ "end" , "11/3/1992"}
			} } },
		{ "city" , "Una ciudad" },
		{ "longitude", 4.44},
		{ "latitude", 2.45},
		{ "resume" , "Test resumen" },
		{ "photo" , tinyJPGBase64 },
	};
	dbj->setDatos(uid, datosJson2);
	Json result2(dbj->getDatos(uid));

	EXPECT_STREQ(result2["resume"].string_value().c_str(), "Test resumen");
	EXPECT_STREQ(result2["email"].string_value().c_str(), "nuevo@mail.com");
	EXPECT_STREQ(result2["birth"].string_value().c_str(), "15/5/1932");
	EXPECT_STREQ(tinyJPGBase64.c_str(), result2["photo"].string_value().c_str());
	EXPECT_EQ(0, result2["contacts"].int_value());
	EXPECT_EQ(0, result1["popularidad"].int_value());
	EXPECT_NEAR(result2["longitude"].number_value(), 4.44, 0.0001);
	EXPECT_NEAR(result2["latitude"].number_value(), 2.45, 0.0001);
	// Skills
	char const * skillArray[] = { "Skill1", "Skill2" };
	int contador = 0;
	for (Json j : result2["skills"].array_items()) {
		EXPECT_STREQ(j.string_value().c_str(), skillArray[contador++]);
	}
	// Puestos
	vector<Puesto> vectorPuesto;
	Fecha fecha1(string("4/11/1994"));
	Fecha fecha2(string("current"));
	vectorPuesto.push_back(Puesto("Puesto1", fecha1, fecha2));
	Fecha fecha3(string("4/2/1990"));
	Fecha fecha4(string("11/3/1992"));
	vectorPuesto.push_back(Puesto("Puesto2", fecha3, fecha4));
	contador = 0;
	for (Json j : result2["job_positions"].array_items()){
		EXPECT_STREQ(j["name"].string_value().c_str(),
				vectorPuesto[contador].puesto.c_str());
		EXPECT_STREQ(j["start"].string_value().c_str(),
				vectorPuesto[contador].fechaInicio.toString().c_str());
		EXPECT_STREQ(j["end"].string_value().c_str(),
				vectorPuesto[contador++].fechaFin.toString().c_str());
	}

}

TEST_F(DBJsonTest, testBrief)
{
	string userName("Username");
	uint32_t uid = registrarTest(userName, 1.0, 0.5);

	Json jBrief = dbj->getDatosBrief(uid);
	EXPECT_STREQ(jBrief["name"].string_value().c_str(), "TNombre TApellido");
	EXPECT_STREQ(jBrief["city"].string_value().c_str(), "Ciudad");
	EXPECT_EQ(0, jBrief["popularidad"].int_value());
	EXPECT_LT(0,jBrief["thumb"].string_value().length());
}

TEST_F(DBJsonTest, testPeticionesYContactos)
{
	string userName1("Username1");
	uint32_t uid1 = registrarTest(userName1, 1.0, 0.5);
	string userName2("Username2");
	uint32_t uid2 = registrarTest(userName2, 2.0, 1.5);
	string userName3("Username3");
	uint32_t uid3 = registrarTest(userName3, 3.0, 2.5);

	EXPECT_EQ(dbj->getContactos(uid1)["contacts"].array_items().size(),0);
	EXPECT_EQ(dbj->getContactos(uid2)["contacts"].array_items().size(),0);
	EXPECT_EQ(dbj->getContactos(uid3)["contacts"].array_items().size(),0);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid1)["count"].int_value(),0);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid2)["count"].int_value(),0);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid3)["count"].int_value(),0);
	Json peticion = Json::object {
		{ "userID" , 0 },
		{ "targetID", 1 },
		{ "message",  "User 0 quiere ser contacto de User 1" },
	};
	dbj->crearPeticion(peticion);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid1)["count"].int_value(),0);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid2)["count"].int_value(),1);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid3)["count"].int_value(),0);
	EXPECT_EQ(dbj->getPeticionesPendientes(uid1)["pending"].array_items().size(), 0);
	EXPECT_EQ(dbj->getPeticionesPendientes(uid2)["pending"].array_items().size(), 1);
	EXPECT_EQ(dbj->getPeticionesPendientes(uid3)["pending"].array_items().size(), 0);
	Json retPeticion = dbj->getPeticion(uid1, uid2);
	EXPECT_THROW(dbj->getPeticion(uid1, uid1), NonexistentRequest);
	EXPECT_THROW(dbj->getPeticion(uid1, uid3), NonexistentRequest);
	EXPECT_THROW(dbj->getPeticion(uid2, uid1), NonexistentRequest);
	EXPECT_THROW(dbj->getPeticion(uid2, uid2), NonexistentRequest);
	EXPECT_THROW(dbj->getPeticion(uid2, uid3), NonexistentRequest);
	EXPECT_THROW(dbj->getPeticion(uid3, uid1), NonexistentRequest);
	EXPECT_THROW(dbj->getPeticion(uid3, uid2), NonexistentRequest);
	EXPECT_THROW(dbj->getPeticion(uid3, uid3), NonexistentRequest);
	EXPECT_THROW(dbj->getPeticion(2589, uid3), NonexistentRequest);
	EXPECT_EQ(retPeticion["userID"].int_value(), uid1);
	EXPECT_EQ(retPeticion["targetID"].int_value(), uid2);
	EXPECT_STREQ(retPeticion["message"].string_value().c_str(),
			"User 0 quiere ser contacto de User 1");
	peticion = Json::object {
		{ "userID" , 2 },
		{ "targetID", 1 },
		{ "message",  "User 2 quiere ser contacto de User 1" },
	};
	dbj->crearPeticion(peticion);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid2)["count"].int_value(),2);
	EXPECT_NO_THROW(dbj->getPeticion(uid3, uid2));
	dbj->declinarPeticion(uid3, uid2);
	EXPECT_THROW(dbj->getPeticion(uid3, uid2), NonexistentRequest);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid2)["count"].int_value(),1);
	dbj->aceptarPeticion(uid1, uid2);
	EXPECT_EQ(dbj->getNumPeticionesPendientes(uid2)["count"].int_value(),0);
	EXPECT_EQ(dbj->getContactos(uid1)["contacts"].array_items().size(),1);
	EXPECT_EQ(dbj->getContactos(uid2)["contacts"].array_items().size(),1);
	EXPECT_TRUE(dbj->esContacto(uid1, uid2));
	EXPECT_TRUE(dbj->esContacto(uid2, uid1));
	EXPECT_FALSE(dbj->esContacto(uid1, uid3));
	EXPECT_FALSE(dbj->esContacto(uid3, uid2));
	EXPECT_EQ(dbj->getContactos(uid2)["contacts"].array_items()[0].int_value(), uid1);
	EXPECT_EQ(dbj->getContactos(uid1)["contacts"].array_items()[0].int_value(), uid2);
	dbj->eliminarContacto(uid2, uid1);
	EXPECT_EQ(dbj->getContactos(uid1)["contacts"].array_items().size(),0);
	EXPECT_EQ(dbj->getContactos(uid2)["contacts"].array_items().size(),0);
	EXPECT_FALSE(dbj->esContacto(uid1, uid2));
	EXPECT_FALSE(dbj->esContacto(uid2, uid1));
}

TEST_F(DBJsonTest, testRecomendaciones)
{
	string userName1("Username1");
	uint32_t uid1 = registrarTest(userName1, 1.0, 0.5);
	string userName2("Username2");
	uint32_t uid2 = registrarTest(userName2, 2.0, 1.5);
	EXPECT_FALSE(dbj->esRecomendado(uid1, uid2)["recommends"].bool_value());
	EXPECT_FALSE(dbj->esRecomendado(uid2, uid1)["recommends"].bool_value());
	Json recomendacion = Json::object {
		{ "recommender" , (int)uid1 },
		{ "recommended", (int)uid2 },
		{ "recommends",  true },
	};
	dbj->actualizarRecomendacion(recomendacion);
	EXPECT_TRUE(dbj->esRecomendado(uid1, uid2)["recommends"].bool_value());
	EXPECT_FALSE(dbj->esRecomendado(uid2, uid1)["recommends"].bool_value());
	recomendacion = Json::object {
		{ "recommender" , (int)uid1 },
		{ "recommended", (int)uid2 },
		{ "recommends",  false },
	};
	dbj->actualizarRecomendacion(recomendacion);
	EXPECT_FALSE(dbj->esRecomendado(uid1, uid2)["recommends"].bool_value());
	EXPECT_FALSE(dbj->esRecomendado(uid2, uid1)["recommends"].bool_value());
}

TEST_F(DBJsonTest, testChat)
{
	string userName1("Username1");
	uint32_t uid1 = registrarTest(userName1, 1.0, 0.5);
	string userName2("Username2");
	uint32_t uid2 = registrarTest(userName2, 2.0, 1.5);
	string userName3("Username3");
	uint32_t uid3 = registrarTest(userName3, 3.0, 2.5);

	EXPECT_EQ(dbj->getChatNuevos(uid1)["new"].array_items().size(), 0);
	Json mensaje = Json::object {
		{ "senderID" , (int)uid2 },
		{ "receiverID", (int)uid1 },
		{ "message",  "Mensaje 0 de 1->0" },
	};
	dbj->enviarMensaje(mensaje);
	mensaje = Json::object {
		{ "senderID" , (int)uid1 },
		{ "receiverID", (int)uid2 },
		{ "message",  "Mensaje 1 de 0->1" },
	};
	dbj->enviarMensaje(mensaje);
	mensaje = Json::object {
		{ "senderID" , (int)uid1 },
		{ "receiverID", (int)uid2 },
		{ "message",  "Mensaje 2 de 0->1" },
	};
	dbj->enviarMensaje(mensaje);
	mensaje = Json::object {
		{ "senderID" , (int)uid3 },
		{ "receiverID", (int)uid2 },
		{ "message",  "Mensaje 1. 2->1" },
	};
	dbj->enviarMensaje(mensaje);
	Json::array chatsNuevos = dbj->getChatNuevos(uid2)["new"].array_items();
	EXPECT_EQ(chatsNuevos.size(), 2);
	EXPECT_EQ(chatsNuevos[0]["senderID"].int_value(), uid1);
	EXPECT_EQ(chatsNuevos[0]["count"].int_value(), 2);
	uint32_t numUltMsg = dbj->getNumLastMensaje(uid1, uid2)["lastmsg"].int_value();
	Json mensajes = dbj->getMensajes(uid1, uid2, 0, numUltMsg);
	EXPECT_EQ(mensajes["messages"][0]["senderID"].int_value(), uid2);
	EXPECT_EQ(mensajes["messages"][1]["senderID"].int_value(), uid1);
	EXPECT_EQ(mensajes["messages"][2]["senderID"].int_value(), uid1);
	EXPECT_EQ(mensajes["messages"][0]["receiverID"].int_value(), uid1);
	EXPECT_EQ(mensajes["messages"][1]["receiverID"].int_value(), uid2);
	EXPECT_EQ(mensajes["messages"][2]["receiverID"].int_value(), uid2);
	EXPECT_STREQ(mensajes["messages"][0]["message"].string_value().c_str(),
			"Mensaje 0 de 1->0");
	EXPECT_STREQ(mensajes["messages"][1]["message"].string_value().c_str(),
			"Mensaje 1 de 0->1");
	EXPECT_STREQ(mensajes["messages"][2]["message"].string_value().c_str(),
			"Mensaje 2 de 0->1");
	EXPECT_EQ(mensajes["messages"][0]["msgID"].int_value(), 0);
	EXPECT_EQ(mensajes["messages"][1]["msgID"].int_value(), 1);
	EXPECT_EQ(mensajes["messages"][2]["msgID"].int_value(), 2);


	mensaje = Json::object {
		{ "senderID" , (int)uid2 },
		{ "receiverID", (int)uid1 },
		{ "message",  "Mensaje 3 de 1->0" },
	};
	dbj->enviarMensaje(mensaje);
	chatsNuevos = dbj->getChatNuevos(uid2)["new"].array_items();
	EXPECT_EQ(chatsNuevos.size(), 1);
}

TEST_F(DBJsonTest, testBusquedaPop)
{
	// Test corto porque la funcionalidad en si se teastea en dbraw
	registrarN(20);
	Json recomendacion = Json::object {
		{ "recommender" , 1 },
		{ "recommended", 0 },
		{ "recommends",  true },
	};
	dbj->actualizarRecomendacion(recomendacion);
	Json pop = dbj->getPopulares();
	EXPECT_EQ(pop["users"].array_items().size(), DBConstNumBusquedaPop);
	EXPECT_EQ(pop["users"].array_items()[0], 0);
	Json datosJson = Json::object {
		{ "name" , "Nombre Test" },
		{ "birth", "4/4/1994" },
		{ "email", "mail1@test.com" },
		{ "skills", Json::array { "Skill1", "Skill2" } },
		{"job_positions",  Json::array { Json::object {
			{ "name", "Puesto1"},
			{ "start", "4/11/1994"},
			{ "end", "current"}
			},
			Json::object {
			{ "name", "Puesto2"},
			{ "start", "4/2/1990"},
			{ "end" , "11/3/1992"}
			} } },
		{ "longitude", 1.0},
		{ "latitude", 0.5},
		{ "city", "Una ciudad" },
	};
	dbj->setDatos(0, datosJson);
	pop = dbj->getPopularesPorSkill("Skill1");
	EXPECT_EQ(pop["users"].array_items().size(), 1);
	EXPECT_EQ(pop["users"].array_items()[0], 0);
	pop = dbj->getPopularesPorPosition("Puesto1");
	EXPECT_EQ(pop["users"].array_items().size(), 1);
	EXPECT_EQ(pop["users"].array_items()[0], 0);
}

TEST(JsonTest, TestJsonChecker)
{
	Json json = Json::object {
				{ "username", "UN" },
				{ "password", defaultBase64PassHash },
	};
	camposExisten(json, "username", "password");
	EXPECT_NO_THROW(camposExisten(json, "username", "password"));
	EXPECT_THROW(camposExisten(json, "no"), BadInputException);
}
