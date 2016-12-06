#include "../include/gtest/gtest.h"
#include "../include/handlers/SharedServerConnector.h"

class SharedServerConnectorTest : public ::testing::Test {
};

TEST_F(SharedServerConnectorTest, get_categories) {
	try {
		Json data = SharedServerConnector::get_categories();
		EXPECT_FALSE(data["categories"].is_null());
	} catch (CurlGetException &e) {}
	catch (InvalidJsonException &e) {}
}

TEST_F(SharedServerConnectorTest, get_job_positions) {
	try {
		Json data = SharedServerConnector::get_job_positions();
		EXPECT_FALSE(data["job_positions"].is_null());
	} catch (CurlGetException &e) {}
	catch (InvalidJsonException &e) {}
}

TEST_F(SharedServerConnectorTest, get_skills) {
	try {
		Json data = SharedServerConnector::get_skills();
		EXPECT_FALSE(data["skills"].is_null());
	} catch (CurlGetException &e) {}
	catch (InvalidJsonException &e) {}
}

TEST_F(SharedServerConnectorTest, invalid_uri) {
	EXPECT_THROW(SharedServerConnector::get(SHARED_SERVER_URL + "/dada"), CurlGetException);
}
