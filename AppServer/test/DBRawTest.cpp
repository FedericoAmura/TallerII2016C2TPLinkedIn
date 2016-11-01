#include "../include/gtest/gtest.h"
#include "../include/gmock/gmock-matchers.h"
#include "../include/database/DBRaw.h"
#include "../include/database/DBExceptions.h"
#include "../include/base64/base64.h"
#include <iostream>
#include <fstream>
#include <cstdio>

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
		db = new DBRaw(dbPath, &std::cout);
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
	db->setDatos(uid, datos);
	datos = db->getDatos(uid);
	EXPECT_STREQ(datos.email.c_str(), "nuevomail@dom.com");
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
	string userName1("TestUserName1");
	string userName2("TestUserName2");
	string userName3("TestUserName3");
	uint32_t uid1 = registrarTest(userName1, 0.5, 0.7);
	uint32_t uid2 = registrarTest(userName2, 0.4, 0.2);
	uint32_t uid3 = registrarTest(userName3, 0.2, 0.1);
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
