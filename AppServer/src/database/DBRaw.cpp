#include "../../include/database/DBRaw.h"
#include "../../include/database/DBExceptions.h"
#include "../../include/log4cpp/OstreamAppender.hh"
#include "../../include/log4cpp/BasicLayout.hh"

using leveldb::Slice;
using leveldb::WriteBatch;
using leveldb::ReadOptions;
using leveldb::WriteOptions;
using leveldb::Status;
using std::vector;
using std::copy;

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


uint32_t DBRaw::registrarse(const DatosUsuario &datos, const string &userName,
		const std::vector<char> &passHash)
{
	vector<char> logKey(sizeof(LOG)+userName.length());
	logKey[0] = LOG;
	copy(userName.begin(), userName.end(), ++logKey.begin());
	Slice logKeySlice(logKey.data(), logKey.size());
	string retVal;
	Status status = db->Get(ReadOptions(), logKeySlice, &retVal);
	if (status.IsNotFound()) { // Nombre de usuario disponible
		WriteBatch batch;
		uint32_t uID = uIDActual();

		// Value de la key para login
		vector<char> logValue(passHash.size()+sizeof(uID));
		vector<char>::iterator it  = copy(passHash.begin(), passHash.end(), logValue.begin());
		copy((char*)&uID, (char*)(&uID+1), it);
		Slice logValueSlice(logValue.data(), logValue.size());

		// Key y value para datos
		vector<char> dataKey(sizeof(USER_DATA)+sizeof(uID));
		dataKey[0] = USER_DATA;
		copy((char*) &uID, (char*) (&uID+1), ++dataKey.begin());
		Slice dataKeySlice(dataKey.data(), sizeof(dataKey));
		vector<char> dataValue = datos.toBytes();
		Slice dataValueSlice(dataValue.data(), sizeof(dataValue));

		// Escribir y aumentar el uID
	    batch.Put(logKeySlice, logValueSlice);
	    batch.Put(dataKeySlice, dataValueSlice);
	    incrementarUID();
	    status = db->Write(WriteOptions(), &batch);
	    verificarEstadoDB(status, "Error al registrar usuario");
		return uID;
	}
	else if(status.ok()) throw PreexistentUsername(userName);
	else verificarEstadoDB(status, "Error al registrar usuario");
}

uint32_t DBRaw::login(const string &userName, const std::vector<char> &passHash) {
	vector<char> logKey(sizeof(LOG)+userName.length());
	logKey[0] = LOG;
	copy(userName.begin(), userName.end(), ++logKey.begin());
	Slice logKeySlice(logKey.data(), logKey.size());
	string retVal;
	Status status = db->Get(ReadOptions(), logKeySlice, &retVal);
	if (status.IsNotFound()) throw NonexistentUsername(userName);
	verificarEstadoDB(status, "Error de DB al hacer login");
	if (strncmp(retVal.data(), passHash.data(), 32) == 0) // Match
	{
		uint32_t uID;
		copy(retVal.data() + 32, retVal.data() + 36, (char*) &uID);
		return uID;
	}
	else throw BadPassword("Password incorrecto.");
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
		char key = LAST_UID;
		char value = 0;
		Status status = db->Put(WriteOptions(), Slice(&key, 1), Slice(&value, 1));
		verificarEstadoDB(status, "Error al inicializar user IDs");
	}
}

uint32_t DBRaw::uIDActual(bool log)
{
	char key = LAST_UID;
	Slice lastIDKey(&key, 1);
	string retVal;
	Status status = db->Get(ReadOptions(), lastIDKey, &retVal);
	verificarEstadoDB(status, "Error al consultar contador de uIDs", log);
	uint32_t lastID = retVal[0];
}

void DBRaw::incrementarUID(WriteBatch* batch)
{
	uint32_t proxUID = uIDActual() + 1;
	char keyBytes = LAST_UID;
	Slice key(&keyBytes, 1);
	Slice value((char*) &proxUID, 4);
	if (batch) batch->Put(key, value);
	else
	{
		Status status = db->Put(WriteOptions(), key, value);
		verificarEstadoDB(status, "Error al incrementar uID", log);
	}
}

void DBRaw::verificarEstadoDB(Status status, const char *mensajeError, bool log)
{
	if (!status.ok()) {
		if (log) dbLog->errorStream() << mensajeError << ": " << status.ToString();
		throw LevelDBException(status.ToString());
	}
}
