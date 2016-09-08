#include "../include/test/SampleTest.h"

SampleTest::SampleTest()
{
    EXPECT_TRUE(true);
}

TEST_F(SampleTest, ASampleTest) 
{
    EXPECT_TRUE(true);
}

SampleTest::~SampleTest() {}
void SampleTest::SetUp() {}
void SampleTest::TearDown() {}
