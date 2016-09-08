#include "../include/test/LevelDBTest.h"

LevelDBTest::LevelDBTest() {}

TEST_F(LevelDBTest, TestCrearUnaBaseDeDatos) {
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "/tmp/testdb", &db);
	EXPECT_TRUE(status.ok());
	delete db;
}
