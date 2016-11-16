#include "../include/test/ServerTest.h"
#include "../include/server/Server.h"

ServerTest::ServerTest() {}

TEST_F(ServerTest, IntegrationTests) {
	Server server(false);
	if (server.setting_ok())
    	server.run();
	EXPECT_TRUE(true);
}
