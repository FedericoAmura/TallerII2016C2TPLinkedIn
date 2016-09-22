#include "../include/test/LibTest.h"

LibTest::LibTest() {}

/**
 * Test de LevelDB correctamente integrado
 */
#include "../include/leveldb/db.h"
TEST_F(LibTest, TestCrearUnaBaseDeDatos) {
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
	EXPECT_TRUE(status.ok());
	delete db;
}

/**
 * Test de log4cpp correctamente integrado
 */
#include "../include/log4cpp/Category.hh"
#include "../include/log4cpp/Appender.hh"
#include "../include/log4cpp/OstreamAppender.hh"
#include "../include/log4cpp/Layout.hh"
#include "../include/log4cpp/BasicLayout.hh"
#include "../include/log4cpp/Priority.hh"
#include <sstream>
TEST_F(LibTest, TestLogger) {
	std::stringstream sstream;
	std::string mensaje("Test");

	log4cpp::Appender *appender = new log4cpp::OstreamAppender("default", &sstream);
	appender->setLayout(new log4cpp::BasicLayout());
	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.setPriority(log4cpp::Priority::WARN);
	root.addAppender(appender);

	root.error(mensaje);
	EXPECT_TRUE(sstream.str().find(mensaje) != std::string::npos);
}

/**
 * Test de libreria de JSon y ejemplo de uso
 */
#include "../include/json11/json11.hpp"
TEST_F(LibTest, TestJSonBuild) {
	json11::Json json = json11::Json::object {
	    { "key1", "strvalue" },
	    { "key2", false },
	    { "key3", json11::Json::array { 1, 2, 3 } },
	};
	EXPECT_STREQ(json["key1"].string_value().c_str(),"strvalue");
	EXPECT_FALSE(json["key2"].bool_value());
	EXPECT_EQ(json["key3"][0].int_value(), 1);
}
TEST_F(LibTest, TestJSonParse) {
	std::string stringToParse("{\"key1\": \"strvalue\"}");
	std::string err;
	json11::Json json = json11::Json::parse(stringToParse, err);
	EXPECT_STREQ(json["key1"].string_value().c_str(),"strvalue");
}
