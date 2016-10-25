#include "../include/gtest/gtest.h"
#include "../include/database/DBJSON.h"
#include "../include/database/DBExceptions.h"
#include <iostream>
#include <cstdio>

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
	void registrarTest(string userName, double latitud, double longitud) {
		json11::Json json = json11::Json::object {
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
		dbj->registrarse(json);
	}
};

TEST_F(DBJsonTest, testRegistroYLogin)
{
	string userName1("TUsername1");
	string userName2("TUsername2");
	registrarTest(userName1, 1.0, 0.5);
	registrarTest(userName2, 0.4, 0.3);
	json11::Json loginJson = json11::Json::object {
				{ "username", userName2 },
				{ "password", defaultBase64PassHash },
	};
	uint32_t uid = dbj->login(loginJson);
	EXPECT_EQ(uid,1);
}
