#include "../include/gtest/gtest.h"
#include "../include/database/DBRaw.h"
#include <iostream>
#include <cstdio>

class RawDBTest : public ::testing::Test {
 public:
	DBRaw *db;
	RawDBTest()
	{
		std::string dbPath("/tmp/testrawdb");
		leveldb::DestroyDB(dbPath, leveldb::Options());
		db = new DBRaw(dbPath, &std::cout);
	}
	~RawDBTest() {delete db;}
};

TEST_F(RawDBTest, testRegistrarPrimerUsuario) {
	EXPECT_TRUE(true);
}

/**
 * Tests sobre tipos asociados
 */

TEST(FechaTest, FechaFromString)
{
	std::string fechaString("21/12/2012");
	Fecha fecha(fechaString);
	EXPECT_EQ(fecha.toUint32_t(), 0x07dc0c15);
}

TEST(FechaTest, FechaFromByteArray)
{
	uint32_t fechaNum = 0x07120519;
	Fecha fecha((char*)&fechaNum);
	std::string fechaString = fecha.toString();
	EXPECT_STREQ("25/5/1810", fechaString.c_str());
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
