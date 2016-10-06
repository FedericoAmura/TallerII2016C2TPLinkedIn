#include "../include/test/JsonParserTest.h"

JsonParserTest::JsonParserTest() {}

TEST_F(JsonParserTest, ParsearRegistroDeNuevoUsuario) {
	json11::Json json = json11::Json::object {
		{"first_name" , "<nombre>"},
		{"last_name" , "<apellido>"},
		{"birth" , "<DD/MM/YYYY>"},
		{"email" , "<email>"},
		{"username" , "<nombre de usuario>"},
		{"password" , "<contraseña>"},
		{"city" , "<ciudad>"},
		{"longitude" , 2.2},
		{"latitude" , 2.5},
	};

	user_record rec;
	bool parsed = JsonParser::parse_user_data(rec, json);
	EXPECT_STREQ(rec.first_name.c_str(), "<nombre>");
	EXPECT_TRUE(parsed);
}

TEST_F(JsonParserTest, ParsearDatosDeNuevoUsuarioDeberiaFallarSiFaltaAlguno) {
	json11::Json json = json11::Json::object {
		//{"first_name" , "nana"},
		{"last_name" , "<apellido>"},
		{"birth" , "<DD/MM/YYYY>"},
		{"email" , "<email>"},
		{"username" , "<nombre de usuario>"},
		{"password" , "<contraseña>"},
		{"city" , "<ciudad>"},
		{"longitude" , 2.2},
		{"latitude" , 2.5},
	};

	user_record rec;
	bool parsed = JsonParser::parse_user_data(rec, json);
	EXPECT_FALSE(parsed);
}

TEST_F(JsonParserTest, ParsearDatosDeActualizacionDePerfilUsuario) {
	json11::Json json = json11::Json::object {
		{
			{"name", "<nombre>"},
			{"skills", json11::Json::array {"<skillname1>", "<skillname2>"}},
			{"job_positions", json11::Json::array { json11::Json::object {
																			{"name","<positionname1>"},
																			{"start", "<DD/MM/YYYY>"},
																			{"end" , "current"},
																		  } ,
													json11::Json::object {
																			{"name","<positionname2>"},
																			{"start", "<DD/MM/YYYY>"},
																			{"end" , "current"},
																		  }
													}
			},
			{"city", "<ciudad>"},
			{"contacts", "<numero>"},
		}
	};

	user_update rec;
	bool parsed = JsonParser::parse_user_update(rec, json);
	EXPECT_TRUE(parsed);
	EXPECT_STREQ(rec.skills[0].c_str(), "<skillname1>");
}

TEST_F(JsonParserTest, ParsearJsonSegunParametro) {
	json11::Json json = json11::Json::object {
		{"summary" , "blabla"},
		{"photo", "AFaSdfsdf3"},
	};

	std::string summary;
	std::string photo;
	bool parsed_summ = JsonParser::parse_parameter(summary, SUMMARY, json);
	bool parsed_photo = JsonParser::parse_parameter(photo, PHOTO, json);
	EXPECT_TRUE(parsed_summ);
	EXPECT_TRUE(parsed_photo);
}

TEST_F(JsonParserTest, ParsearSolicitudDeContacto) {
	json11::Json json = json11::Json::object {
		{"sender_id" , "pepe"},
		{"target_id" , "lalo"},
		{"message" , "hola"},
	};

	contact_request rec;
	bool parsed = JsonParser::parse_contact_request(rec, json);
	EXPECT_TRUE(parsed);
}

TEST_F(JsonParserTest, ParsearSolicitudDeRecomendacion) {
	json11::Json json = json11::Json::object {
		{"recommender" , "pepe"},
		{"recommended" , "lalo"},
		{"recommends" , true},
	};

	recommend_request rec;
	bool parsed = JsonParser::parse_recommend_request(rec, json);
	EXPECT_TRUE(parsed);
	EXPECT_TRUE(rec.recommends);
}
