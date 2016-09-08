#include "../include/gtest/gtest.h"
#include "../include/leveldb/db.h"

// Fixture para testear que levelDB fue exitosamente integrado al proyecto
class LevelDBTest : public ::testing::Test 
{
protected:
    LevelDBTest();
};
