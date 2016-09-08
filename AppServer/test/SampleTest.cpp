#include "../include/test/SampleTest.h"

SampleTest::SampleTest() {}

TEST_F(SampleTest, ASampleTest) {
    EXPECT_TRUE(true);
}

SampleTest::~SampleTest() {}
void SampleTest::SetUp() {}
void SampleTest::TearDown() {}
