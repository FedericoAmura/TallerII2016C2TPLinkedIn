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
#include "log4cpp/Category.hh"
#include "log4cpp/Appender.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Priority.hh"
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
