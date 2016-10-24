#include "../../include/database/DBJSON.h"
#include "../../include/database/DatosUsuario.h"
#include "../../include/base64/base64.h"

DBJSON::DBJSON(SharedServerHandler* sharedServerHandler, DBRaw *db) :
	sharedServerHandler(sharedServerHandler), db(db) {}

DBJSON::~DBJSON() {
	delete db;
}

uint32_t DBJSON::registrarse(const Json &json) {
	//TODO: Chequeo de existencia de campos
	string nombre = json["first_name"].string_value();
	nombre.append(" ");
	nombre.append(json["last_name"].string_value());
	string email = json["email"].string_value();
	string ciudad = json["city"].string_value();
	Fecha nacimiento(json["birth"].string_value());
	double longitud = json["longitude"].number_value();
	double latitud = json["latitude"].number_value();
	Geolocacion geo(longitud, latitud);
	DatosUsuario datos(nombre, email, ciudad, nacimiento, geo);

	string userName = json["user_name"].string_value();
	string passHashStr = base64_decode(json["pass"].string_value());
	std::vector<char> passHash(passHashStr.begin(), passHashStr.end());
	return db->registrarse(datos, userName, passHash);
}


uint32_t DBJSON::login(const Json &json) {
	//TODO: Chequeo de existencia de campos
	string userName = json["username"].string_value();
	string passHashStr = base64_decode(json["password"].string_value());
	std::vector<char> passHash(passHashStr.begin(), passHashStr.end());
	return db->login(userName, passHash);
}

Json DBJSON::getDatos(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

Json DBJSON::getDatosBrief(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

void DBJSON::setDatos(uint32_t userID, const Json &json) {

}





