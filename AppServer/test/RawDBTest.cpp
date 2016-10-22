#include "../include/gtest/gtest.h"
#include "../include/database/DBRaw.h"
#include <iostream>

class RawDBTest : public ::testing::Test {
 public:
	DBRaw db;
	RawDBTest() : db(std::string("/tmp/testdb"), &std::cout) {};
};

TEST_F(RawDBTest, testRegistrarPrimerUsuario) {
	EXPECT_TRUE(true);
}

/**
 * Tests sobre tipos asociados
 */

TEST(FechaTest, FechaFromString)
{
	std::string fechaString("2012/12/21");
	Fecha fecha(fechaString);
	EXPECT_EQ(fecha.toUint32_t(), 0x07DC0c15);
}

TEST(FechaTest, FechaFromByteArray)
{
	uint32_t fechaNum = 0x07120519;
	Fecha fecha((char*)&fechaNum);
	std::string fechaString = fecha.toString();
	EXPECT_STREQ("1810/5/25", fechaString.c_str());
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
