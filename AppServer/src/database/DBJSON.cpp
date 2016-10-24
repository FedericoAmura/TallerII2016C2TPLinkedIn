#include "../../include/database/DBJSON.h"

DBJSON::DBJSON(SharedServerHandler* shared, DBRaw *DBRaw) {
	this->sharedServerHandler = shared;
	this->db = DBRaw;
}

DBJSON::~DBJSON() {
	delete db;
}

uint32_t DBJSON::registrarse(const Json &json) {
	return 0;
}


uint32_t DBJSON::login(const Json &json) {
	return 0;
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





