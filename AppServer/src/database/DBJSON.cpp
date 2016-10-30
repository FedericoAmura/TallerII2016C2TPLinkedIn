#include "../../include/database/DBJSON.h"
#include "../../include/database/DatosUsuario.h"
#include "../../include/base64/base64.h"
#include "../../include/database/JsonChecker.h"

DBJSON::DBJSON(SharedServerHandler* sharedServerHandler, DBRaw *db) :
	sharedServerHandler(sharedServerHandler), db(db) {}

DBJSON::~DBJSON() {
	delete db;
	tokens.clear();
}

string DBJSON::generarToken(const Json &json) {
	string username = json["username"].string_value();
	string current_timestamp = get_current_timestamp();
	string to_hash = username + "|" + current_timestamp;
	string token_hash = base64_encode((const unsigned char*)to_hash.c_str(), to_hash.length());
	tokens[token_hash] = current_timestamp;
	return token_hash;
}

bool DBJSON::validar_token(const string &token) {
	double EXPIRATION_TIME_SEC = 86400; // 86400 sec == 1 día
	if (tokens.count(token) > 0) {
		string timestamp = tokens[token];
		double diff = time_difference_seconds(timestamp);
		if (diff > EXPIRATION_TIME_SEC) {
			tokens.erase(token);
			throw TokenHasExpired("Token ha expirado");
		} else
			return true;
	} else
		throw NonexistentToken("Token inexistente");
}

uint32_t DBJSON::registrarse(const Json &json) {
	camposExisten(json, "first_name", "last_name", "email", "city",
			"birth", "longitude", "latitude");
	string nombre(json["first_name"].string_value());
	nombre.append(" ");
	nombre.append(json["last_name"].string_value());
	string email(json["email"].string_value());
	string ciudad(json["city"].string_value());
	Fecha nacimiento(json["birth"].string_value());
	double longitud = json["longitude"].number_value();
	double latitud = json["latitude"].number_value();
	Geolocacion geo(longitud, latitud);
	DatosUsuario datos(nombre, email, ciudad, nacimiento, geo);

	string userName = json["username"].string_value();
	string passHashStr = base64_decode(json["password"].string_value());
	vector<char> passHash(passHashStr.begin(), passHashStr.end());
	return db->registrarse(datos, userName, passHash);
}


uint32_t DBJSON::login(const Json &json) {
	camposExisten(json, "username", "password");
	string userName = json["username"].string_value();
	string passHashStr = base64_decode(json["password"].string_value());
	vector<char> passHash(passHashStr.begin(), passHashStr.end());
	return db->login(userName, passHash);
}

Json DBJSON::getDatos(uint32_t userID) {
	DatosUsuario datos = db->getDatos(userID);
	string resumen = db->getResumen(userID);
	Foto foto = db->getFoto(userID);
	uint16_t contacts = db->getNumContactos(userID);
	int popularidad = db->getPopularidad(userID);
	vector<string> skillVector(db -> getSkills(userID));
	Json::array skills(skillVector.begin(), skillVector.end());
	vector<Puesto> puestosVector(db -> getPuestos(userID));
	Json::array puestos;
	for (Puesto p : puestosVector)
	{
		Json j = Json::object {
			{ "name" , p.puesto},
			{ "start", p.fechaInicio.toString()},
			{ "end", p.fechaFin.toString() }
		};
		puestos.push_back(j);
	}
	Json data = Json::object {
		{"name" , datos.nombre},
		{"city" , datos.ciudad},
		{"skills", skills},
		{"job_positions", puestos},
		{"contacts", contacts },
		{"popularidad", popularidad},
		{"resume" , resumen},
		{"photo" , foto.toBase64String()},
	};
	return data;
}

Json DBJSON::getDatosBrief(uint32_t userID) {
	DatosUsuario datos = db->getDatos(userID);
	int popularidad = db->getPopularidad(userID);
	Foto thumb = db->getFotoThumbnail(userID);
	Json data = Json::object {
		{ "name", datos.nombre },
		{ "popularidad", popularidad },
		{ "city", datos.ciudad },
		{ "thumb", thumb.toBase64String() },
	};
	return data;
}

void DBJSON::setDatos(uint32_t userID, const Json &json) {
	camposExisten(json, "name", "skills", "job_positions", "city");
	DatosUsuario datos = db->getDatos(userID);
	datos.nombre = json["name"].string_value();
	datos.ciudad = json["city"].string_value();
	vector<string> skills;
	for (Json j : json["skills"].array_items())
	{
		skills.push_back(j.string_value());
	}
	vector<Puesto> puestos;
	for (Json j : json["job_positions"].array_items())	{
		camposExisten(j, "name", "start", "end");
		Puesto p(j["name"].string_value(),
				Fecha(j["start"].string_value()),
				Fecha(j["end"].string_value()));
		puestos.push_back(p);
	}
	string resumen;
	string *pResumen = NULL;
	if (!json["resume"].is_null())
	{
		resumen = json["resume"].string_value();
		pResumen = &resumen;
	}
	if (!json["photo"].is_null())
	{
		Foto photo(json["photo"].string_value());
		db->setPerfil(userID, datos, skills, puestos, pResumen, &photo);
	}
	else
		db->setPerfil(userID, datos, skills, puestos, pResumen, NULL);
}

Json DBJSON::getResumen(uint32_t userID) {
	string resumen(db->getResumen(userID));
	Json data = Json::object { {"resume", resumen} };
	return data;
}

void DBJSON::setResumen(uint32_t userID, const Json &json) {
	camposExisten(json, "resume");
	string resumen(json["resume"].string_value());
	db->setResumen(userID, resumen);
}

Json DBJSON::getFoto(uint32_t userID) {
	string foto(db->getFoto(userID).toBase64String());
	Json data = Json::object { {"photo", foto} };
	return data;
}

Json DBJSON::getFotoThumbnail(uint32_t userID) {
	string foto(db->getFotoThumbnail(userID).toBase64String());
	Json data = Json::object { {"thumb", foto} };
	return data;
}

void DBJSON::setFoto(uint32_t userID, const Json &json) {
	camposExisten(json, "photo");
	Foto foto(json["photo"].string_value());
	db->setFoto(userID, foto);
}

Json DBJSON::busqueda_profresional(const vector<string>
	*puestos, const vector<string> *skill, const vector<string>
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

