#include "../../include/database/DBRaw.h"
#include "../../include/database/DBExceptions.h"
#include "../../include/log4cpp/OstreamAppender.hh"
#include "../../include/log4cpp/BasicLayout.hh"

using leveldb::Slice;
using leveldb::ReadOptions;
using leveldb::WriteOptions;
using leveldb::Status;

enum KeyCode : uint8_t
{
	LAST_UID,
    LOG,
	USER_DATA,
	US_RESUMEN,
	US_FOTO,
	FOTO,
	FOTO_THUMB,
	US_POS,
	US_SKILL,
	RL_POS,
	RL_SKILLS,
	US_CONTACT,
	US_CONTACT_PAIR,
	US_CONTACT_COUNT,
	SOLIC,
	SOLIC_TXT,
	US_POP,
	US_POP_PAIR,
	RL_POP,
	GEO_US,
	CONV_COUNT,
	CONV_MSG,
	CONV_LAST_READ,
	CONV_PENDING_READ
};

DBRaw::DBRaw(const std::string& rutaArchivo, std::ostream *logStream)
	: logStream(logStream) {
	dbLogAppender = new log4cpp::OstreamAppender("dbAppender", logStream);
	dbLogAppender->setLayout(new log4cpp::BasicLayout());
	dbLog = &log4cpp::Category::getInstance(std::string("dbLog"));
	dbLog->addAppender(dbLogAppender);

	leveldb::Options options;
	options.create_if_missing = true;
	Status status = leveldb::DB::Open(options, rutaArchivo, &db);
	verificarEstadoDB(status,  "Error al abrir la base de datos");
	inicializarUID();
}

DBRaw::~DBRaw() {
	dbLog->shutdown();
}


uint32_t registrarse(const DatosUsuario &datos, const string &userName,
		const std::vector<uint8_t> &passHash)
{
	/*ReadOptions readOptions;
	Slice lastIDKey([LAST_UID], 1);
	string retVal;
	db->Get(readOptions, lastIDKey, &retVal);
	uint8_t lastID = retVal[0];*/
}

/*
void DBRaw::setFoto(uint32_t uID, Foto& foto) {
}

void DBRaw::setGeolocacion(uint32_t uID, Geolocacion geolocacion) {
}

void DBRaw::setResumen(uint32_t uID, const string& resumen) {
}

DatosUsuario DBRaw::getDatos(uint32_t uID) {
}

void DBRaw::setDatos(uint32_t uID, const DatosUsuario& datos) {
}

Foto DBRaw::getFoto(uint32_t uID) {
}

Foto DBRaw::getFotoThumbnail(uint32_t uID) {
}

uint32_t DBRaw::login(const string& username, const uint8_t* passwordHash) {
}

std::vector<uint32_t> DBRaw::busquedaProfresional(
		const std::vector<string>* puestos, const std::vector<string>* skill,
		const std::vector<string>* categorias, Geolocacion* geolocacion,
		float maxDist, bool sortPopularidad) {
}

std::vector<uint32_t> DBRaw::busquedaPopular(uint conteo) {
}

std::vector<uint32_t> DBRaw::busquedaPopularSkill(const string& skill,
		uint conteo) {
}

std::vector<uint32_t> DBRaw::busquedaPopularPuesto(const string& puesto,
		uint conteo) {
}

void DBRaw::solicitarContacto(uint32_t uIDFuente, uint32_t uIDDestino,
		const string& mensaje) {
}

std::vector<uint32_t> DBRaw::getSolicitudes(uint32_t uIDConsultador) {
}

string DBRaw::getMsgSolicitud(uint32_t uIDFuente, uint32_t uIDDestino) {
}

void DBRaw::aceptarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino) {
}

void DBRaw::eliminarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino) {
}

void DBRaw::eliminarContacto(uint32_t uID1, uint32_t uID2) {
}

std::vector<uint32_t> DBRaw::getContactos(uint32_t uID) {
}

uint DBRaw::getNumContactos(uint32_t uID) {
}

uint32_t DBRaw::numUltMensaje(uint32_t uID1, uint32_t uID2) {
}

std::vector<std::pair<uint32_t, string> > DBRaw::getMensajes(uint32_t uID1,
		uint32_t uID2, uint32_t numUltMensaje, uint32_t numPrimMensaje) {
}*/

void DBRaw::inicializarUID()
{
	try
	{
		uIDActual(false);
	}
	catch (LevelDBException &e)
	{
		WriteOptions writeOptions;
		char key = LAST_UID;
		char value = 0;
		Status status = db->Put(writeOptions, Slice(&key, 1), Slice(&value, 1));
		verificarEstadoDB(status, "Error al inicializar user IDs");
	}
}

uint32_t DBRaw::uIDActual(bool log)
{
	ReadOptions readOptions;
	char key = LAST_UID;
	Slice lastIDKey(&key, 1);
	string retVal;
	Status status = db->Get(readOptions, lastIDKey, &retVal);
	verificarEstadoDB(status, "Error al consultar contador de uIDs", log);
	uint32_t lastID = retVal[0];
}

void DBRaw::incrementarUID()
{
	uint32_t proxUID = uIDActual() + 1;
	WriteOptions writeOptions;
	char keyBytes = LAST_UID;
	Slice key(&keyBytes, 1);
	Slice value((char*) &proxUID, 4);
	Status status = db->Put(writeOptions, key, value);
	verificarEstadoDB(status, "Error al incrementar user IDs");
}

void DBRaw::verificarEstadoDB(Status status, const char *mensajeError, bool log)
{
	if (!status.ok()) {
		if (log) dbLog->errorStream() << mensajeError << ": " << status.ToString();
		throw LevelDBException(status.ToString());
	}
}
