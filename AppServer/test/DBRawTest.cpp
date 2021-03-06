#include "../include/gtest/gtest.h"
#include "../include/gmock/gmock-matchers.h"
#include "../include/database/DBRaw.h"
#include "../include/database/DBExceptions.h"
#include "../include/database/DBConstants.h"
#include "../include/base64/base64.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

/**
 * Tests sobre la clase DBRaw
 */

static const string defaultFecha("15/10/1945");
static const string defaultNombre("Test Name");
static const string defaultEmail("email@dominio.com");
static const string defaultCiudad("Ciudad");
static const std::vector<char> defaultPassHash = {
	0x15, 0x0c, 0x1c, 0x07, 0x01, 0x3a, 0x5b, 0x26,
	0x15, 0x0c, 0x1c, 0x07, 0x01, 0x3a, 0x5b, 0x26,
	0x15, 0x0c, 0x1c, 0x07, 0x01, 0x3a, 0x5b, 0x26,
	0x15, 0x0c, 0x1c, 0x07, 0x01, 0x3a, 0x5b, 0x26
};
static const std::vector<unsigned char> tinyJPG = {
		0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46,
		0x00, 0x01, 0x01, 0x01, 0x00, 0x48, 0x00, 0x48, 0x00, 0x00,
		0xFF, 0xDB, 0x00, 0x43, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xC2, 0x00, 0x0B, 0x08, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01,
		0x11, 0x00, 0xFF, 0xC4, 0x00, 0x14, 0x10, 0x01, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0xFF, 0xDA, 0x00, 0x08, 0x01, 0x01,
		0x00, 0x01, 0x3F, 0x10
};
static const std::string tinyJPGBase64 = "/9j/4AAQSkZJRgABAQEASABIA"
		"AD/2wBDAP/////////////////////////////////////////////////"
		"/////////////////////////////////////wgALCAABAAEBAREA/8QAF"
		"BABAAAAAAAAAAAAAAAAAAAAAP/aAAgBAQABPxA=";
static const string dbPath("/tmp/testrawdb");


class DBRawTest : public ::testing::Test {
 public:
	DBRaw *db;
	DBRawTest()
	{
		leveldb::Status status = leveldb::DestroyDB(dbPath, leveldb::Options());
		if(!status.ok()) std::cout << "Error elminando DB: " << status.ToString() << std::endl;
		db = new DBRaw(dbPath);
	}
	~DBRawTest() {
		delete db;
	}
	/**
	 * Registra un usuario con varios parametros por defecto
	 * @param userName				Nombre de usuario
	 * @param latitud				Latitud
	 * @param longitud				Longitud
	 * @return						El user ID del recien registrado
	 */
	uint32_t registrarTest(string userName, double latitud, double longitud)	{
		Fecha fecha(defaultFecha);
		Geolocacion geo(latitud, longitud);
		DatosUsuario datos(defaultNombre, defaultEmail, defaultCiudad, fecha, geo);
		return db->registrarse(datos, userName, defaultPassHash);
	}
	void registrarN(int n, int offset = 0){
		for (int i = offset; i < n+offset; ++i)
		{
			string username = string("Username").append(std::to_string(i));
			registrarTest(username, (double)i, (double)i);
		}
	}
};

TEST_F(DBRawTest, testRegistroYLogin)
{
	string userName1("TestUserName1");
	string userName2("TestUserName2");
	registrarTest(userName1, 0.5, 0.7);
	registrarTest(userName2, 0.4, 0.2);
	uint32_t uid = db->login(userName1, defaultPassHash);
	EXPECT_EQ(uid,0);
	uid = db->login(userName2, defaultPassHash);
	EXPECT_EQ(uid,1);
	EXPECT_THROW(db->login(string("Bad Username"), defaultPassHash), NonexistentUsername);
	EXPECT_THROW({
		std::vector<char> passHash(defaultPassHash);
		passHash[0] = 34;
		uint32_t uid = db->login(userName1, passHash);
	}, BadPassword);
}

TEST_F(DBRawTest, testGetSetDatos) {
	EXPECT_THROW(db->getDatos(0), NonexistentUserID);
	string userName("TestUserName");
	registrarTest(userName, 0.4, 0.6);
	int32_t uid = db->login(userName, defaultPassHash);

	DatosUsuario datos = db->getDatos(uid);
	EXPECT_STREQ(datos.nombre.c_str(), defaultNombre.c_str());
	EXPECT_STREQ(datos.email.c_str(), defaultEmail.c_str());
	EXPECT_STREQ(datos.ciudad.c_str(), defaultCiudad.c_str());
	EXPECT_STREQ(datos.fechaNacimiento.toString().c_str(), defaultFecha.c_str());
	EXPECT_DOUBLE_EQ(0.4, datos.geolocacion.longitud());
	EXPECT_DOUBLE_EQ(0.6, datos.geolocacion.latitud());
	EXPECT_EQ(0, datos.fotoID);

	datos.email = "nuevomail@dom.com";
	datos.nombre = "Nuevo Nombre";
	datos.fechaNacimiento = Fecha(string("3/3/1333"));
	db->setDatos(uid, datos);
	datos = db->getDatos(uid);
	EXPECT_STREQ(datos.nombre.c_str(), "Nuevo Nombre");
	EXPECT_STREQ(datos.email.c_str(), "nuevomail@dom.com");
	EXPECT_STREQ(datos.fechaNacimiento.toString().c_str(), "3/3/1333");
}

TEST_F(DBRawTest, testGetSetResumen) {
	EXPECT_THROW(db->getResumen(0), NonexistentUserID);
	string userName("TestUserName");
	registrarTest(userName, 0.2, 0.1);
	int32_t uID = db->login(userName, defaultPassHash);
	EXPECT_STREQ("", db->getResumen(uID).c_str());

	string resumen("Este es el contenido de un resumen.");
	db->setResumen(uID, resumen);
	EXPECT_STREQ(resumen.c_str(), db->getResumen(uID).c_str());
}

TEST_F(DBRawTest, testGetSetPuestos) {
	EXPECT_THROW(db->getPuestos(0), NonexistentUserID);
	string userName("TestUserName");
	registrarTest(userName, 0.4, 0.2);
	int32_t uID = db->login(userName, defaultPassHash);
	std::vector<Puesto> puestos(db->getPuestos(uID));
	EXPECT_EQ(puestos.size(), 0);
	Puesto puesto1(string("Trabajo1"), string("22/2/1995"), string("23/4/1996"));
	Puesto puesto2(string("Trabajo2"), string("5/1/1996"), string("11/12/1999"));
	puestos.push_back(puesto1);
	puestos.push_back(puesto2);
	db->setPuestos(uID, puestos);
	puestos = db->getPuestos(uID);
	EXPECT_STREQ(puestos[0].puesto.c_str(), "Trabajo1");
	EXPECT_STREQ(puestos[0].fechaInicio.toString().c_str(), "22/2/1995");
	EXPECT_STREQ(puestos[0].fechaFin.toString().c_str(), "23/4/1996");
	EXPECT_STREQ(puestos[1].puesto.c_str(), "Trabajo2");
	EXPECT_STREQ(puestos[1].fechaInicio.toString().c_str(), "5/1/1996");
	EXPECT_STREQ(puestos[1].fechaFin.toString().c_str(), "11/12/1999");
}

TEST_F(DBRawTest, testGetSetSkills) {
	EXPECT_THROW(db->getSkills(0), NonexistentUserID);
	string userName("TestUserName");
	registrarTest(userName, 0.6, 0.25);
	int32_t uID = db->login(userName, defaultPassHash);

	std::vector<string> skills(db->getSkills(uID));
	EXPECT_EQ(skills.size(), 0);
	skills.push_back("Skill1");
	skills.push_back("Skill2");
	skills.push_back("Skill3");
	db->setSkills(uID, skills);
	skills = db->getSkills(uID);
	EXPECT_STREQ(skills[0].c_str(), "Skill1");
	EXPECT_STREQ(skills[1].c_str(), "Skill2");
	EXPECT_STREQ(skills[2].c_str(), "Skill3");
}

TEST_F(DBRawTest, testGetSetFoto) {
	EXPECT_THROW(db->getFoto(0), NonexistentUserID);
	EXPECT_THROW(db->getFotoThumbnail(0), NonexistentUserID);
	string userName("TestUserName");
	registrarTest(userName, 0.6, 0.25);
	int32_t uID = db->login(userName, defaultPassHash);

	std::ifstream fotoFile(defaultFotoPath);
	EXPECT_TRUE(fotoFile.is_open());
	string content = string(std::istreambuf_iterator<char>(fotoFile),
			std::istreambuf_iterator<char>());
	std::vector<char> bytes = db->getFoto(uID).toBytes();
	EXPECT_EQ(content.length(), bytes.size());
	EXPECT_THAT(content, ::testing::ElementsAreArray(bytes));
	bytes = db->getFotoThumbnail(uID).toBytes();
	EXPECT_LE(content.length(), bytes.size());

	Foto foto(tinyJPGBase64);
	db->setFoto(uID, foto);
	bytes = db->getFoto(uID).toBytes();
	EXPECT_EQ(tinyJPG.size(), bytes.size());
	EXPECT_THAT(tinyJPG, ::testing::ElementsAreArray(bytes));
}

TEST_F(DBRawTest, testContactos) {
	registrarN(3);
	uint32_t uid1 = 0;
	uint32_t uid2 = 1;
	uint32_t uid3 = 2;
	db->solicitarContacto(uid1, uid2, "User 1° pide ser contacto de User 2°");
	db->solicitarContacto(uid3, uid2, "User 3° pide ser contacto de User 2°");
	vector <uint32_t> pending = db->getSolicitudes(uid2);
	uint16_t numContactos = db->getNumContactos(uid2);
	EXPECT_EQ(0, numContactos);
	EXPECT_EQ(pending[0], uid1);
	EXPECT_EQ(pending[1], uid3);
	EXPECT_EQ(2, pending.size());
	EXPECT_EQ(2, (int)db->getNumSolicitudes(uid2));
	EXPECT_STREQ(db->getMsgSolicitud(uid1, uid2).c_str(),
			"User 1° pide ser contacto de User 2°");
	EXPECT_STREQ(db->getMsgSolicitud(uid3, uid2).c_str(),
			"User 3° pide ser contacto de User 2°");
	EXPECT_THROW(db->getMsgSolicitud(uid1, 1000), NonexistentRequest);
	EXPECT_THROW(db->eliminarSolicitud(uid2, uid3), NonexistentRequest);
	db->eliminarSolicitud(uid3, uid2);
	pending = db->getSolicitudes(uid2);
	EXPECT_EQ(1, pending.size());
	EXPECT_EQ(1, (int)db->getNumSolicitudes(uid2));
	db->aceptarSolicitud(uid1, uid2);
	pending = db->getSolicitudes(uid2);
	EXPECT_EQ(0, pending.size());
	EXPECT_EQ(0, (int)db->getNumSolicitudes(uid2));
	numContactos = db->getNumContactos(uid2);
	EXPECT_EQ(1, numContactos);
	EXPECT_THROW(db->solicitarContacto(uid1, uid2, ""), AlreadyContacts);
	EXPECT_THROW(db->solicitarContacto(uid2, uid1, ""), AlreadyContacts);
	EXPECT_THROW(db->solicitarContacto(uid1, uid1, ""), AlreadyContacts);
	vector <uint32_t> contactos = db->getContactos(uid2);
	EXPECT_EQ(contactos[0], uid1);
	contactos = db->getContactos(uid1);
	EXPECT_EQ(contactos[0], uid2);
	EXPECT_THROW(db->solicitarContacto(uid1, uid2, ""),  AlreadyContacts);
	EXPECT_TRUE(db->sonContactos(uid1,uid2));
	EXPECT_TRUE(db->sonContactos(uid2,uid1));
	EXPECT_FALSE(db->sonContactos(uid1,uid3));
	EXPECT_FALSE(db->sonContactos(uid3,uid2));
	db->eliminarContacto(uid1, uid2);
	EXPECT_FALSE(db->sonContactos(uid1,uid2));
	EXPECT_EQ(0, db->getNumContactos(uid1));
	EXPECT_EQ(0, db->getNumContactos(uid2));
}

TEST_F(DBRawTest, testRecomendaciones) {
	registrarN(3);
	uint32_t uid1 = 0;
	uint32_t uid2 = 1;
	uint32_t uid3 = 2;
	EXPECT_EQ(db->getPopularidad(uid1), 0);
	EXPECT_EQ(db->getPopularidad(uid1), 0);
	EXPECT_EQ(db->getPopularidad(uid1), 0);
	db->setRecomendacion(uid1, uid2, true);
	EXPECT_EQ(db->getPopularidad(uid2), 1);
	db->setRecomendacion(uid1, uid2, true); // Ignorado
	EXPECT_EQ(db->getPopularidad(uid2), 1);
	db->setRecomendacion(uid1, uid2, false);
	EXPECT_EQ(db->getPopularidad(uid2), 0);
	db->setRecomendacion(uid1, uid2, false); // Ignorado
	EXPECT_EQ(db->getPopularidad(uid2), 0);
	db->setRecomendacion(uid1, uid1, true); // Ignorado
	db->setRecomendacion(uid2, uid1, true);
	db->setRecomendacion(uid2, uid3, true);
	db->setRecomendacion(uid3, uid1, true);
	EXPECT_EQ(db->getPopularidad(uid1), 2);
	EXPECT_EQ(db->getPopularidad(uid2), 0);
	EXPECT_EQ(db->getPopularidad(uid3), 1);
	EXPECT_FALSE(db->esRecomendado(uid1, uid1));
	EXPECT_FALSE(db->esRecomendado(uid1, uid2));
	EXPECT_FALSE(db->esRecomendado(uid1, uid3));
	EXPECT_TRUE(db->esRecomendado(uid2, uid1));
	EXPECT_FALSE(db->esRecomendado(uid2, uid2));
	EXPECT_TRUE(db->esRecomendado(uid2, uid3));
	EXPECT_TRUE(db->esRecomendado(uid3, uid1));
	EXPECT_FALSE(db->esRecomendado(uid3, uid2));
	EXPECT_FALSE(db->esRecomendado(uid3, uid3));
}

TEST_F(DBRawTest, testChat) {
	registrarN(3);
	uint32_t uid1 = 0;
	uint32_t uid2 = 1;
	uint32_t uid3 = 2;
	EXPECT_EQ(db->getConversacionesNoLeidas(uid1).size(), 0);
	db->enviarMensaje(uid1, uid2, "Mensaje 0. 2->1");
	EXPECT_EQ(db->getConversacionesNoLeidas(uid1).size(), 1);
	EXPECT_EQ(db->getNumUltMensaje(uid1, uid2), 1);
	std::pair<uint32_t, string> mensaje = db->getMensajes(uid1, uid2, 0, 0)[0];
	EXPECT_EQ(mensaje.first, uid2);
	EXPECT_STREQ(mensaje.second.c_str(), "Mensaje 0. 2->1");
	db->marcarConversacionLeida(uid1, uid2);
	EXPECT_EQ(db->getConversacionesNoLeidas(uid1).size(), 0);
	EXPECT_EQ(db->getNumUltMensaje(uid1, uid2), 1);
	db->enviarMensaje(uid1, uid2, "Mensaje 1. 2->1");
	db->enviarMensaje(uid2, uid1, "Mensaje 2. 1->2");
	db->enviarMensaje(uid1, uid2, "Mensaje 3. 2->1");
	db->enviarMensaje(uid1, uid3, "Mensaje de user 3->1");
	vector<uint32_t> pending = db->getConversacionesNoLeidas(uid1);
	EXPECT_EQ(pending.size(), 2);
	mensaje = db->getMensajes(uid1, uid3, 0, 0)[0];
	EXPECT_EQ(mensaje.first, uid3);
	EXPECT_STREQ(mensaje.second.c_str(), "Mensaje de user 3->1");
	db->marcarConversacionLeida(uid1, uid3);
	pending = db->getConversacionesNoLeidas(uid1);
	EXPECT_EQ(pending.size(), 1);
	uint32_t uIDSender = pending[0];
	uint32_t finConvo = db->getNumUltMensaje(uid1, uIDSender);
	uint32_t ultimoLeido = db->getUltimoMsgNoLeido(uid1, uIDSender);
	vector<std::pair<uint32_t, string>> mensajes
		= db->getMensajes(uid1, uid2, 1, finConvo);
	EXPECT_EQ(mensajes[0].first, uid2);
	EXPECT_EQ(mensajes[1].first, uid1);
	EXPECT_EQ(mensajes[2].first, uid2);
	EXPECT_STREQ(mensajes[0].second.c_str(), "Mensaje 1. 2->1");
	EXPECT_STREQ(mensajes[1].second.c_str(), "Mensaje 2. 1->2");
	EXPECT_STREQ(mensajes[2].second.c_str(), "Mensaje 3. 2->1");
}

TEST_F(DBRawTest, testBusquedaPopularidad) {
	EXPECT_EQ(db->busquedaPopular().size(), 0);
	EXPECT_EQ(db->busquedaPopularSkill("Skill1").size(), 0);
	EXPECT_EQ(db->busquedaPopularPuesto("Puesto1").size(), 0);
	registrarN(5);
	std::vector<uint32_t> pop = db->busquedaPopular();
	EXPECT_EQ(pop.size(), 5);
	registrarN(DBConstNumBusquedaPop*2, 5);
	EXPECT_EQ(db->busquedaPopular().size(), DBConstNumBusquedaPop);
	EXPECT_EQ(db->busquedaPopularSkill("Skill1").size(), 0);
	EXPECT_EQ(db->busquedaPopularPuesto("Puesto1").size(), 0);
	db->setSkills(1, vector<string>{"Skill1"});
	db->setSkills(5, vector<string>{"Skill2"});
	db->setRecomendacion(1,0, true);
	db->setRecomendacion(2,0, true);
	db->setRecomendacion(4,1, true);
	pop = db->busquedaPopular();
	EXPECT_EQ(pop.size(), DBConstNumBusquedaPop);
	EXPECT_EQ(pop[0], 0);
	EXPECT_EQ(pop[1], 1);
	// Skills
	pop = db->busquedaPopularSkill("Skill1");
	EXPECT_EQ(pop.size(), 1);
	EXPECT_EQ(pop[0], 1);
	pop = db->busquedaPopularSkill("Skill2");
	EXPECT_EQ(pop.size(), 1);
	EXPECT_EQ(pop[0], 5);
	db->setSkills(0, vector<string>{"Skill1"});
	pop = db->busquedaPopularSkill("Skill1");
	EXPECT_EQ(pop.size(), 2);
	EXPECT_EQ(pop[0], 0);
	EXPECT_EQ(pop[1], 1);
	db->setRecomendacion(1,0, false);
	db->setRecomendacion(2,0, false);
	pop = db->busquedaPopularSkill("Skill1");
	EXPECT_EQ(pop.size(), 2);
	EXPECT_EQ(pop[0], 1);
	EXPECT_EQ(pop[1], 0);
	db->setSkills(0, vector<string>());
	pop = db->busquedaPopularSkill("Skill1");
	EXPECT_EQ(pop.size(), 1);
	EXPECT_EQ(pop[0], 1);
	for (int i = 0; i < DBConstNumBusquedaPop*2; ++i)
		db->setSkills(i, vector<string>{"Skill4"});
	pop = db->busquedaPopularSkill("Skill4");
	EXPECT_EQ(pop.size(), DBConstNumBusquedaPop);
	// Puestos
	pop = db->busquedaPopularPuesto("Puesto1");
	EXPECT_EQ(pop.size(), 0);
	Puesto p("Puesto1", defaultFecha, Fecha(string("23/3/2222")));
	db->setPuestos(7, vector<Puesto>{ p });
	pop = db->busquedaPopularPuesto("Puesto1");
	EXPECT_EQ(pop.size(), 1);
	EXPECT_EQ(pop[0], 7);
	db->setPuestos(7, vector<Puesto>{});
	pop = db->busquedaPopularPuesto("Puesto1");
	EXPECT_EQ(pop.size(),0);
}

TEST_F(DBRawTest, testBusquedaProfesional) {
	registrarN(20);
	db->setRecomendacion(0, 6, true);
	db->setRecomendacion(1, 6, true);
	db->setRecomendacion(2, 6, true);
	db->setRecomendacion(3, 6, true);
	db->setRecomendacion(4, 6, true);
	db->setRecomendacion(0, 7, true);
	db->setRecomendacion(1, 7, true);
	db->setRecomendacion(2, 7, true);
	db->setRecomendacion(7, 8, true);
	// Todos los usuarios del sistema
	vector<uint32_t> result = db->busquedaProfesional(NULL, NULL, NULL, 0, true);
	EXPECT_EQ(result.size(), 20);
	EXPECT_EQ(result[0], 6);
	EXPECT_EQ(result[1], 7);
	EXPECT_EQ(result[2], 8);
	// Por skillss
	vector<string> skill1Vec = {"Skill1"};
	vector<string> skill2Vec = {"Skill2"};
	vector<string> skill12Vec = {"Skill1","Skill2"};
	db->setSkills(6, skill12Vec);
	db->setSkills(7, skill1Vec);
	db->setSkills(8, skill2Vec);
	db->setSkills(0, skill12Vec);
	result = db->busquedaProfesional(NULL, &skill1Vec, NULL, 0, true);
	EXPECT_EQ(result.size(), 3);
	EXPECT_EQ(result[0], 6);
	EXPECT_EQ(result[1], 7);
	EXPECT_EQ(result[2], 0);
	result = db->busquedaProfesional(NULL, &skill2Vec, NULL, 0, true);
	EXPECT_EQ(result.size(), 3);
	EXPECT_EQ(result[0], 6);
	EXPECT_EQ(result[1], 8);
	EXPECT_EQ(result[2], 0);
	result = db->busquedaProfesional(NULL, &skill12Vec, NULL, 0, true);
	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], 6);
	EXPECT_EQ(result[1], 0);
	// Por puestos
	vector<string> puesto1Vec = {"Puesto1"};
	vector<string> puesto2Vec = {"Puesto2"};
	vector<string> puesto12Vec = {"Puesto1","Puesto2"};
	Fecha f1(string("21/12/2000"));
	Fecha f2(string("22/12/2000"));
	db->setPuestos(6, vector<Puesto> {Puesto("Puesto1",f1,f2), Puesto("Puesto2",f1,f2)});
	db->setPuestos(7, vector<Puesto> {Puesto("Puesto1",f1,f2)} );
	db->setPuestos(0, vector<Puesto> {Puesto("Puesto1",f1,f2), Puesto("Puesto2",f1,f2)});
	result = db->busquedaProfesional(&puesto1Vec, NULL, NULL, 0, true);
	EXPECT_EQ(result.size(), 3);
	EXPECT_EQ(result[0], 6);
	EXPECT_EQ(result[1], 7);
	EXPECT_EQ(result[2], 0);
	result = db->busquedaProfesional(&puesto12Vec, NULL, NULL, 0, true);
	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], 6);
	EXPECT_EQ(result[1], 0);
	result = db->busquedaProfesional(&puesto1Vec, &skill2Vec, NULL, 0, true);
	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], 6);
	EXPECT_EQ(result[1], 0);
	// Locacion geografica
	Geolocacion geo0(0.0, 0.0);
	Geolocacion geo5(5.0, 5.0);
	Geolocacion geo6(6.0, 6.0);
	result = db->busquedaProfesional(&puesto1Vec, &skill2Vec, &geo5, 2, true);
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result[0], 6);
	result = db->busquedaProfesional(NULL, NULL, &geo0, 0, false);
	EXPECT_EQ(result.size(), 1);
	result = db->busquedaProfesional(NULL, NULL, &geo0, 1.5, false);
	EXPECT_EQ(result.size(), 2);
	result = db->busquedaProfesional(NULL, NULL, &geo5, 0.01, false);
	EXPECT_EQ(result.size(), 1);
	result = db->busquedaProfesional(NULL, NULL, &geo5, 5, false);
	EXPECT_EQ(result.size(), 7);
}

/**
 * Tests sobre tipos asociados
 */

TEST(FechaTest, FechaFromString)
{
	Fecha fecha1(std::string("21/12/2012"));
	EXPECT_EQ(fecha1.toUint32_t(), 0x07dc0c15);
	Fecha fecha2(std::string("current"));
	EXPECT_EQ(fecha2.toUint32_t(), 0xffffffff);
}

TEST(FechaTest, FechaFromByteArray)
{
	uint32_t fechaNum = 0x07120519;
	Fecha fecha((char*)&fechaNum);
	EXPECT_STREQ("25/5/1810", fecha.toString().c_str());
	fechaNum = 0xffffffff;
	Fecha fecha2((char*)&fechaNum);
	EXPECT_STREQ("current", fecha2.toString().c_str());
}

TEST(FechaTest, FechaFromFecha)
{
	std::string fechaString("1/2/2003");
	Fecha fecha1(fechaString);
	Fecha fecha2(fecha1);
	EXPECT_EQ(fecha1.toUint32_t(), fecha2.toUint32_t());
}

TEST(GeoTest, GeoFromDouble)
{
	Geolocacion geo(0.756, 0.8);
	EXPECT_DOUBLE_EQ(0.756, geo.longitud());
	EXPECT_DOUBLE_EQ(0.8, geo.latitud());
}

TEST(GeoTest, GeoFromByteArray)
{
	double array[] = {0.756, 0.8};
	Geolocacion geo((char*)array);
	EXPECT_DOUBLE_EQ(0.756, geo.longitud());
	EXPECT_DOUBLE_EQ(0.8, geo.latitud());
}

TEST(GeoTest, GeoToByteArray)
{
	double array[] = {0.756, 0.8};
	Geolocacion geoBase((char*)array);
	Geolocacion geo(geoBase.toBytes().data());
	EXPECT_DOUBLE_EQ(0.756, geo.longitud());
	EXPECT_DOUBLE_EQ(0.8, geo.latitud());
}


TEST(GeoTest, GeoFromGeo)
{
	Geolocacion geo(0.4, 0.3);
	Geolocacion geo2(geo);
	EXPECT_DOUBLE_EQ(0.4, geo2.longitud());
	EXPECT_DOUBLE_EQ(0.3, geo2.latitud());
}

TEST(DatosTest, DatosFromParameter)
{
	Fecha fecha(string("21/12/2012"));
	Geolocacion geo(0.12, 0.3);
	DatosUsuario datos("Juan Tester", "email@dominio.com", "Ciudad", fecha, geo);
	EXPECT_STREQ(datos.nombre.c_str(), "Juan Tester");
	EXPECT_STREQ(datos.email.c_str(), "email@dominio.com");
	EXPECT_STREQ(datos.ciudad.c_str(), "Ciudad");
	EXPECT_DOUBLE_EQ(0.12, datos.geolocacion.longitud());
	EXPECT_DOUBLE_EQ(0.3, datos.geolocacion.latitud());
	EXPECT_STREQ("21/12/2012", datos.fechaNacimiento.toString().c_str());
}

TEST(DatosTest, DatosFromByteArray)
{
	// Se asume arquitectura little endian. TODO: Verificar y poner un if
	uint8_t byteArray[] = { 11, 'J', 'u', 'a', 'n', ' ', 'T', 'e', 's', 't', 'e',
			'r'	, 17, 'e', 'm', 'a', 'i', 'l', '@', 'd', 'o', 'm', 'i', 'n',
			'i', 'o', '.', 'c', 'o', 'm', 6, 'C', 'i', 'u', 'd', 'a', 'd',
			0x15, 0x0c, 0xdc, 0x07, // Fecha
			0xb8, 0x1e, 0x85, 0xeb, 0x51, 0xb8, 0xbe, 0x3f, // Latitud
			0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xd3, 0x3f, // Longitud
			0x01, 0xca, 0xab, 0xff }; //FotoID
	DatosUsuario datos((char*)byteArray);
	EXPECT_STREQ(datos.nombre.c_str(), "Juan Tester");
	EXPECT_STREQ(datos.email.c_str(), "email@dominio.com");
	EXPECT_STREQ(datos.ciudad.c_str(), "Ciudad");
	EXPECT_STREQ("21/12/2012", datos.fechaNacimiento.toString().c_str());
	EXPECT_DOUBLE_EQ(0.12, datos.geolocacion.longitud());
	EXPECT_DOUBLE_EQ(0.3, datos.geolocacion.latitud());
	EXPECT_EQ(0xffabca01, datos.fotoID);
}

TEST(DatosTest, DatosToByteArray)
{
	Fecha fecha(string("21/12/2012"));
	Geolocacion geo(0.12, 0.3);
	DatosUsuario datosBase("Juan Tester", "email@dominio.com", "Ciudad", fecha, geo, 0xffabca01);
	DatosUsuario datos(datosBase.toBytes().data());
	EXPECT_STREQ(datos.nombre.c_str(), "Juan Tester");
	EXPECT_STREQ(datos.email.c_str(), "email@dominio.com");
	EXPECT_STREQ(datos.ciudad.c_str(), "Ciudad");
	EXPECT_STREQ("21/12/2012", datos.fechaNacimiento.toString().c_str());
	EXPECT_DOUBLE_EQ(0.12, datos.geolocacion.longitud());
	EXPECT_DOUBLE_EQ(0.3, datos.geolocacion.latitud());
	EXPECT_EQ(0xffabca01, datos.fotoID);
}

TEST(FotoTest, DatosFromString)
{
	Foto foto(tinyJPGBase64);
	std::vector<char> bytes(foto.toBytes());
	std::vector<unsigned char> charBytes(bytes.size());
	std::copy(bytes.begin(), bytes.end(), charBytes.begin());
	EXPECT_THAT(tinyJPG, ::testing::ContainerEq(charBytes));
}

TEST(FotoTest, DatosFromByteArray)
{
	std::vector<char> bytes(tinyJPG.size());
	std::copy(tinyJPG.begin(), tinyJPG.end(), bytes.begin());
	Foto foto(bytes.data(), bytes.size());
	EXPECT_TRUE(tinyJPGBase64.compare(foto.toBase64String()) == 0);
}

TEST(PuestoTest, DatosFromString)
{
	Fecha fInicio(string("2/4/1991"));
	Fecha fFin(string("3/7/1992"));
	Puesto puesto("UnPuesto", fInicio, fFin);
	EXPECT_STREQ(puesto.puesto.c_str(), "UnPuesto");
	EXPECT_STREQ(puesto.fechaInicio.toString().c_str(), "2/4/1991");
	EXPECT_STREQ(puesto.fechaFin.toString().c_str(), "3/7/1992");
	EXPECT_THROW(Puesto puesto("UnPuesto", fFin, fInicio), TimeTravel);
}

TEST(PuestoTest, DatosFromByteArray)
{
	unsigned char byteArray[] = { 6, 'P', 'u', 'e', 's', 't','o',
			0x15, 0x0c, 0xdc, 0x07, 0xff, 0xff, 0xff, 0xff };
	Puesto puesto((char*)byteArray);
	EXPECT_STREQ(puesto.puesto.c_str(), "Puesto");
	EXPECT_STREQ(puesto.fechaInicio.toString().c_str(), "21/12/2012");
	EXPECT_STREQ(puesto.fechaFin.toString().c_str(), "current");
}

#include "../include/database/JpegWrapper.h"
TEST(JpegTest, TestResize)
{
	EXPECT_TRUE(true);; // TODO: Implementar
	/*std::ifstream fotoFile("default.jpg");
	string content = string(std::istreambuf_iterator<char>(fotoFile),
			std::istreambuf_iterator<char>());
	std::vector<char> bytes(tinyJPG.size());
	std::copy(content.begin(), content.end(), bytes.begin());

	FotoData rgbResult = descomprimir(bytes);
	for (char c : rgbResult.data)
		std::cout << std::hex << c << " ";*/
}
