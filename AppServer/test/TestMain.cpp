#include "../include/gtest/gtest.h"
#include "../include/common/Logger.h"

int main(int argc, char **argv) {
	Logger::setLevel(ERROR);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
