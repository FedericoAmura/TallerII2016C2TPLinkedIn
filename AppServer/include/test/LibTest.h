#ifndef APPSERVER_INCLUDE_TEST_LIBTEST_H_
#define APPSERVER_INCLUDE_TEST_LIBTEST_H_

#include "../include/gtest/gtest.h"

// Fixture para testear que librerias varias fueron exitosamente integrado al proyecto
class LibTest : public ::testing::Test {
 protected:
    LibTest();
};

#endif  // APPSERVER_INCLUDE_TEST_LIBTEST_H_
