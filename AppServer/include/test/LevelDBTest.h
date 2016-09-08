#ifndef APPSERVER_INCLUDE_TEST_LEVELDBTEST_H_
#define APPSERVER_INCLUDE_TEST_LEVELDBTEST_H_

#include "../include/gtest/gtest.h"
#include "../include/leveldb/db.h"

// Fixture para testear que levelDB fue exitosamente integrado al proyecto
class LevelDBTest : public ::testing::Test {
 protected:
    LevelDBTest();
};

#endif  // APPSERVER_INCLUDE_TEST_LEVELDBTEST_H_
