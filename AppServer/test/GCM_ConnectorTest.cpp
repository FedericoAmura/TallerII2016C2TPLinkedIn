#include "../include/gtest/gtest.h"
#include "../include/handlers/GCM_Connector.h"

class GCM_ConnectorTest : public ::testing::Test {
};

TEST_F(GCM_ConnectorTest, hacerPushNotificationConDatosInvalidos) {
	std::string data = "data";
	EXPECT_THROW(GCM_Connector::notify(data), RequestRejected);
}
