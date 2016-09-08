#ifndef APPSERVER_INCLUDE_TEST_SAMPLETEST_H_
#define APPSERVER_INCLUDE_TEST_SAMPLETEST_H_

#include "../include/gtest/gtest.h"

// Test fixture de ejemplo
class SampleTest : public ::testing::Test {
 protected:
    // Aca se puede hacer setup vario.
    // Notar que la clase se crea y se destruye por cada test.
    SampleTest();

    // Limpiar cosas que no tiren excepciones
    virtual ~SampleTest();

    // Codigo que se llama inmediatamente despues del constructor
    // y antes de cada test
    virtual void SetUp();

    // Codigo que se llama inmediatamente antes del destructor
    // y despues de cada test
    virtual void TearDown();
};

#endif  // APPSERVER_INCLUDE_TEST_SAMPLETEST_H_
