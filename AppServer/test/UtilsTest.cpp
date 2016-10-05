#include "../include/test/UtilsTest.h"
#include "../include/common/Utils.h"

UtilsTest::UtilsTest() {}

TEST_F(UtilsTest, ValidacionDeUriValida) {
	std::string uri_s = "/users/<user_id>";
	URI uri = validate_uri(uri_s);
	EXPECT_EQ(uri, _USER_PROFILE);
}

TEST_F(UtilsTest, ValidacionDeUriInvalida) {
	std::string uri_s = "/user_s/<user_id>";
	URI uri = validate_uri(uri_s);
	EXPECT_EQ(uri, _INVALID_URI);
}

TEST_F(UtilsTest, ConvertirMETHODaString) {
	METHOD method = _GET;
	std::string s_method = MethodtoString(method);
	EXPECT_STREQ(s_method.c_str(), "GET");

	method = _POST;
	s_method = MethodtoString(method);
	EXPECT_STREQ(s_method.c_str(), "POST");

	method = _PUT;
	s_method = MethodtoString(method);
	EXPECT_STREQ(s_method.c_str(), "PUT");

	method = _DELETE;
	s_method = MethodtoString(method);
	EXPECT_STREQ(s_method.c_str(), "DELETE");
}

TEST_F(UtilsTest, SplitSobreUnaCadena) {
	std::string cadena = "hola, cómo estás?, todo bien?";
	std::vector<std::string> vec = split(cadena, ",");
	EXPECT_STREQ(vec[0].c_str(), "hola");
	EXPECT_EQ(vec.size(), 3);
}
