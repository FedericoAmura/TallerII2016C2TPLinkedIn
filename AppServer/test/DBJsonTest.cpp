#include "../include/gtest/gtest.h"
#include "../include/database/DBJSON.h"
#include "../include/database/DBExceptions.h"
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
		dbj = new DBJSON(NULL, db);
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
	registrarTest(userName, 1.0, 0.5);
	Json loginJson = Json::object {
				{ "username", userName },
				{ "password", defaultBase64PassHash },
	};
	uint32_t uid = dbj->login(loginJson);

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
	registrarTest(userName, 1.0, 0.5);
	Json loginJson = Json::object {
				{ "username", userName },
				{ "password", defaultBase64PassHash },
	};
	uint32_t uid = dbj->login(loginJson);

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
	registrarTest(userName, 1.0, 0.5);
	Json loginJson = Json::object {
				{ "username", userName },
				{ "password", defaultBase64PassHash },
	};
	uint32_t uid = dbj->login(loginJson);

	// Prueba can escases de datos
	Json datosJson1 = Json::object {
				{ "name" , "Nombre Test" },
				{ "skills", Json::array { } },
				{"job_positions",  Json::array { } },
				{ "city" , "Una ciudad" },
			};
	dbj->setDatos(uid, datosJson1);
	Json result1(dbj->getDatos(uid));

	EXPECT_STREQ(result1["resume"].string_value().c_str(), "");
	EXPECT_LT(tinyJPGBase64.length(), result1["photo"].string_value().length());
	EXPECT_EQ(0, result1["contacts"].int_value());
	EXPECT_EQ(0, result1["popularidad"].int_value());
	EXPECT_TRUE(result1["skills"].array_items().empty());
	EXPECT_TRUE(result1["puestos"].array_items().empty());

	// Prueba con abundancia de datos
	Json datosJson2 = Json::object {
			{ "name" , "Nombre Test" },
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
			{ "resume" , "Test resumen" },
			{ "photo" , tinyJPGBase64 },
		};
	dbj->setDatos(uid, datosJson2);
	Json result2(dbj->getDatos(uid));

	EXPECT_STREQ(result2["resume"].string_value().c_str(), "Test resumen");
	EXPECT_STREQ(tinyJPGBase64.c_str(), result2["photo"].string_value().c_str());
	EXPECT_EQ(0, result2["contacts"].int_value());
	EXPECT_EQ(0, result1["popularidad"].int_value());
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
	registrarTest(userName, 1.0, 0.5);
	Json loginJson = Json::object {
				{ "username", userName },
				{ "password", defaultBase64PassHash },
	};
	uint32_t uid = dbj->login(loginJson);

	Json jBrief = dbj->getDatosBrief(uid);
	EXPECT_STREQ(jBrief["name"].string_value().c_str(), "TNombre TApellido");
	EXPECT_STREQ(jBrief["city"].string_value().c_str(), "Ciudad");
	EXPECT_EQ(0, jBrief["popularidad"].int_value());
	EXPECT_LT(0,jBrief["thumb"].string_value().length());

}

TEST(JsonTest, TestJsonChecker)
{
	Json json = Json::object {
				{ "username", "UN" },
				{ "password", defaultBase64PassHash },
	};
	camposExisten(json, "username", "password");
	EXPECT_THROW(camposExisten(json, "no"), BadInputException);
}
