#include "../../include/database/DBRaw.h"
#include "../../include/database/DBExceptions.h"
#include "../../include/log4cpp/OstreamAppender.hh"
#include "../../include/log4cpp/BasicLayout.hh"

DBRaw::DBRaw(const std::string& rutaArchivo, std::ostream *logStream)
	: logStream(logStream) {
	dbLogAppender = new log4cpp::OstreamAppender("dbAppender", logStream);
	dbLogAppender->setLayout(new log4cpp::BasicLayout());
	dbLog = &log4cpp::Category::getInstance(std::string("dbLog"));
	dbLog->addAppender(dbLogAppender);

	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, rutaArchivo, &db);
	if (!status.ok()) {
		dbLog->errorStream() << "Error al abrir la base de datos: "
				<< status.ToString();
		throw LevelDBException(status.ToString());
	}
}

DBRaw::~DBRaw() {
	dbLog->shutdown();
	delete logStream;
	delete db;
}/*

uint32_t DBRaw::registrarse() {
}

void DBRaw::agregarFoto(uint32_t uID, void* foto) {
}

void DBRaw::agregarSkill(uint32_t uID, const std::string& skill) {
}

void DBRaw::eliminarSkill(uint32_t uID, const std::string& skill) {
}

void DBRaw::agregarPuestoLaboral(uint32_t uID, const std::string& puesto,
		uint32_t fechaInicio, uint32_t fechaFin) {
}

void DBRaw::eliminarPosicion(uint32_t uID, const std::string& puesto,
		uint32_t fechaInicio, uint32_t fechaFin) {
}

void DBRaw::actualizarEMail(uint32_t uID, const std::string& email) {
}

void DBRaw::actualizarCiudad(uint32_t uID, const std::string& ciudad) {
}

void DBRaw::actualizarGeolocacion(uint32_t uID, float latitud, float longitud) {
}

void DBRaw::actualizarresumen(uint32_t uID, const std::string& resumen) {
}

int DBRaw::getDatos(uint32_t uID) {
}

void* DBRaw::getFoto(uint32_t uID) {
}

void* DBRaw::getFotoThumbnail(uint32_t uID) {
}

uint32_t DBRaw::login(const std::string& username, const void* passwordHash) {
}

std::vector<uint32_t> DBRaw::busqueda_profresional(
		std::vector<std::string>& puestos, std::vector<std::string>& skill,
		const std::vector<std::string>& categorias,
		std::pair<float, float> geolocacion, float maxDist,
		bool sortPopularidad) {
}

std::vector<uint32_t> DBRaw::busqueda_popularidad(uint conteo,
		std::vector<std::string>& puestos, std::vector<std::string>& skill,
		const std::vector<std::string>& categorias) {
}

void DBRaw::solicitarContacto(uint32_t uIDFuente, uint32_t uIDDestino,
		const std::string& mensaje) {
}

void DBRaw::aceptarContacto(uint32_t uIDFuente, uint32_t uIDDestino) {
}

void DBRaw::declinarContactor(uint32_t uIDFuente, uint32_t uIDDestino) {
}

std::vector<uint32_t> DBRaw::getContactos(uint32_t uID) {
}

uint DBRaw::getNumContactos(uint32_t uID) {
}

uint32_t DBRaw::numUltMensaje(uint32_t uID1, uint32_t uID2) {
}

std::vector<std::pair(uint32_t, std::string)> DBRaw::getMensajes(uint32_t uID1,
		uint32_t uID2, uint32_t numUltMensaje, uint32_t numPrimMensaje) {
}
*/
