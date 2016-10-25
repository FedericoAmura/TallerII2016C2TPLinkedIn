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

Json DBJSON::getResumen(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

void DBJSON::setResumen(uint32_t userID, const Json &json) {
}

Json DBJSON::getFoto(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

Json DBJSON::getFotoThumbnail(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

void DBJSON::setFoto(uint32_t userID, const Json &json) {
}

Json DBJSON::busqueda_profresional(const std::vector<string>
	*puestos, const std::vector<string> *skill, const std::vector<string>
	*categorias, Geolocacion *geolocacion, float maxDist,
	bool sortPopularidad) {

	Json data = Json::object {};
	return data;
}

Json DBJSON::getPeticionesPendientes(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

Json DBJSON::getNumPeticionesPendientes(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

Json DBJSON::getPeticion(uint32_t userID1, uint32_t userID2) {
	Json data = Json::object {};
	return data;
}

void DBJSON::aceptarPeticion(uint32_t userID1, uint32_t userID2) {
}

void DBJSON::declinarPeticion(uint32_t userID1, uint32_t userID2) {
}

Json DBJSON::getContactos(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

void DBJSON::crearPeticion(const Json &json) {
}

bool DBJSON::esContacto(uint32_t userID1, uint32_t userID2) {
	return false;
}

void DBJSON::eliminarContacto(uint32_t userID1, uint32_t userID2) {
}

Json DBJSON::esRecomendado(uint32_t userIDRecomendador, uint32_t userIDRecomendado) {
	Json data = Json::object {};
	return data;
}

void DBJSON::actualizarRecomendacion(const Json &json) {
}

Json DBJSON::getPopulares() {
	Json data = Json::object {};
	return data;
}

Json DBJSON::getPopularesPorSkill(const string &skill) {
	Json data = Json::object {};
	return data;
}

Json DBJSON::getPopularesPorPosition(const string &position) {
	Json data = Json::object {};
	return data;
}

Json DBJSON::getChatNuevos(uint32_t userID) {
	Json data = Json::object {};
	return data;
}

void DBJSON::marcarChatLeido(const Json &json) {
}

void DBJSON::enviarMensaje(const Json &json) {
}
