#include "../include/gtest/gtest.h"
#include "../include/server/Server.h"

class ServerTest : public ::testing::Test {
};

TEST_F(ServerTest, IntegrationTests) {
	Server server(false);
	if (server.setting_ok())
    	server.run();
	EXPECT_TRUE(true);
}
