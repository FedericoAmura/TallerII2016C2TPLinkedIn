#include "../../include/database/DBRaw.h"
#include "../../include/database/DBExceptions.h"
#include "../../include/database/DBConstants.h"
#include "../../include/log4cpp/OstreamAppender.hh"
#include "../../include/log4cpp/BasicLayout.hh"
#include <fstream>

using leveldb::Slice;
using leveldb::WriteBatch;
using leveldb::ReadOptions;
using leveldb::WriteOptions;
using leveldb::Status;
using std::vector;
using std::copy;
using std::string;

enum KeyCode : uint8_t
{
	LAST_UID,
    LOG,
	USER_DATA,
	US_RESUMEN,
	LAST_FID,
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

/**
 * Clase para simplificar la creacion de slices de tipo prefijo[1]uint32_t[4]
 */
class IDKey {
public:
 std::vector<char> data;
 IDKey(KeyCode prefijo, uint32_t key) : data(5) {
	 data[0] = prefijo;
	 copy((char*)&key, (char*)(&key+1), data.begin()+1);
 }
 Slice toSlice() {
	 return Slice (data.data(), data.size());
 }
};

DBRaw::DBRaw(const string& rutaArchivo, std::ostream *logStream)
	: logStream(logStream) {
	/*TODO es temporal el siguiente Destroy */
	leveldb::DestroyDB(rutaArchivo, leveldb::Options());
	dbLogAppender = new log4cpp::OstreamAppender("dbAppender", logStream);
	dbLogAppender->setLayout(new log4cpp::BasicLayout());
	dbLog = &log4cpp::Category::getInstance(string("dbLog"));
	dbLog->addAppender(dbLogAppender);

	leveldb::Options options;
	options.create_if_missing = true;
	Status status = leveldb::DB::Open(options, rutaArchivo, &db);
	verificarEstadoDB(status,  "Error al abrir la base de datos");
	inicializarContador(LAST_UID, string("user ID"));
	inicializarFID(defaultFotoPath);
}

DBRaw::~DBRaw() {
	dbLog->shutdown();
	delete db;
}

uint32_t DBRaw::registrarse(const DatosUsuario &datos, const string &userName,
		const std::vector<char> &passHash)
{
	if (passHash.size() != 32) throw BadPasswordSize("La password debe ser de 32 bytes.");
	//TODO: Lock para evitar errores RAW con el ID
	vector<char> logKey(sizeof(LOG)+userName.length());
	logKey[0] = LOG;
	copy(userName.begin(), userName.end(), logKey.begin()+1);
	Slice logKeySlice(logKey.data(), logKey.size());
	string retVal;
	Status status = db->Get(ReadOptions(), logKeySlice, &retVal);
	if (status.IsNotFound()) { // Nombre de usuario disponible
		WriteBatch batch;
		uint32_t uID = contadorActual(LAST_UID, string("user ID"));
		// Value de la key para login
		vector<char> logValue(passHash.size()+sizeof(uID));
		vector<char>::iterator it  = copy(passHash.begin(), passHash.end(), logValue.begin());
		copy((char*)&uID, (char*)(&uID+1), it);
		Slice logValueSlice(logValue.data(), logValue.size());
		batch.Put(logKeySlice, logValueSlice);
		// Key y value para datos
		setDatos(uID, datos, &batch, false);
		// Inicializamos otros campos
		setResumen(uID, string(""), &batch, false);
		setPuestos(uID, std::vector<Puesto>(), &batch, false);
		setSkills(uID, std::vector<string>(), &batch, false);
		// Escribir y aumentar el uID
	    incrementarContador(LAST_UID, string("user ID"),&batch);
	    status = db->Write(WriteOptions(), &batch);
	    verificarEstadoDB(status, "Error 1 al registrar usuario");
		return uID;
	}
	else if(status.ok()) throw PreexistentUsername(userName);
	else verificarEstadoDB(status, "Error 2 al registrar usuario");
}

uint32_t DBRaw::login(const string &userName, const std::vector<char> &passHash) {
	if (passHash.size() != 32) throw BadPasswordSize("La password debe ser de 32 bytes.");
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

void DBRaw::setDatos(uint32_t uID, const DatosUsuario& datos, WriteBatch *batch, bool verifUID) {
	if (verifUID) verificarContador<NonexistentUserID>(LAST_UID, string("user ID"),	uID);
	vector<char> dataValue = datos.toBytes();
	Slice dataValueSlice(dataValue.data(), dataValue.size());
	IDKey dataKey(USER_DATA, uID);
	// TODO : Reverse lookup geoloc
	if (batch) batch->Put(dataKey.toSlice(), dataValueSlice);
	else {
		Status status = db->Put(WriteOptions(), dataKey.toSlice(), dataValueSlice);
		verificarEstadoDB(status, "Error al guardar datos de usuario");
	}
}

DatosUsuario DBRaw::getDatos(uint32_t uID) {
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	string retVal;
	IDKey datosKey(USER_DATA, uID);
	Status status = db->Get(ReadOptions(), datosKey.toSlice(), &retVal);
	if (status.IsNotFound()) throw NonexistentUserID(std::to_string(uID));
	verificarEstadoDB(status, "Error al obtener datos de usuario");
	return DatosUsuario(retVal.data());
}

void DBRaw::setResumen(uint32_t uID, const string& resumen,
		WriteBatch *batch, bool verifUID) {
	if (verifUID) verificarContador<NonexistentUserID>(LAST_UID, string("user ID"),uID);
	IDKey resumenKey(US_RESUMEN, uID);
	if (batch) batch->Put(resumenKey.toSlice(), Slice(resumen));
	else {
		Status status = db->Put(WriteOptions(), resumenKey.toSlice(), Slice(resumen));
		verificarEstadoDB(status, "Error al guardar el resumen");
	}
}

string DBRaw::getResumen(uint32_t uID) {
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	string retVal;
	IDKey resumenKey(US_RESUMEN, uID);
	Status status = db->Get(ReadOptions(), resumenKey.toSlice(), &retVal);
	verificarEstadoDB(status, "Error al obtener datos de usuario");
	return retVal;
}

void DBRaw::setSkills(uint32_t uID, std::vector<string> skills,
		leveldb::WriteBatch *batch, bool verifUID) {
	if (verifUID) verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	vector<char> bytes;
	for (string sk : skills) {
		// TODO: Verificar contra el shared
		bytes.push_back(sk.length());
		std::copy(sk.begin(), sk.end(), back_inserter(bytes));
	}
	IDKey skillsKey(US_SKILL, uID);
	Slice data(bytes.data(), bytes.size());
	if (batch) batch->Put(skillsKey.toSlice(), data);
	else {
		Status status = db->Put(WriteOptions(), skillsKey.toSlice(), data);
		verificarEstadoDB(status, "Error al guardar skills");
	}
	// TODO: Agregar a reverse lookup
}

vector<string> DBRaw::getSkills(uint32_t uID) {
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	IDKey skillsKey(US_SKILL, uID);
	string retVal;
	vector<string> result;
	size_t contador = 0;
	Status status = db->Get(ReadOptions(), skillsKey.toSlice(), &retVal);
	verificarEstadoDB(status, "Error al consultar skills.");
	while (contador < retVal.length())
	{
		uint8_t len = retVal[contador];
		string skill(retVal.data()+contador+1, len);
		result.push_back(skill);
		contador += (1+skill.length());
	}
	return result;
}

void DBRaw::setPuestos(uint32_t uID, std::vector<Puesto> puestos,
		leveldb::WriteBatch *batch, bool verifUID) {
	if (verifUID) verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	vector<char> bytes;
	for (Puesto p : puestos) {
		// TODO: Verificar contra el shared
		vector<char> pBytes = p.toBytes();
		bytes.insert(bytes.end(), pBytes.begin(), pBytes.end());
	}
	IDKey puestosKey(US_POS, uID);
	Slice data(bytes.data(), bytes.size());
	if (batch) batch->Put(puestosKey.toSlice(), data);
	else {
		Status status = db->Put(WriteOptions(), puestosKey.toSlice(), data);
		verificarEstadoDB(status, "Error al guardar puestos");
	}
	// TODO: Agregar a reverse lookup
}

vector<Puesto> DBRaw::getPuestos(uint32_t uID) {
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	IDKey puestosKey(US_POS, uID);
	string retVal;
	vector<Puesto> result;
	size_t contador = 0;
	Status status = db->Get(ReadOptions(), puestosKey.toSlice(), &retVal);
	verificarEstadoDB(status, "Error al consultar puestos.");
	while (contador < retVal.length())
	{
		Puesto puesto(retVal.data()+contador);
		result.push_back(puesto);
		contador += (1+puesto.length());
	}
	return result;
}

void DBRaw::setFoto(uint32_t uID, const Foto &foto,
		WriteBatch *batch, bool verifUID) {
	// TODO: Lock para evitar errores RAW con el ID
	if (verifUID) verificarContador<NonexistentUserID>(LAST_UID, string("user ID"),uID);
	WriteBatch localBatch;
	bool writeToDB = !batch;
	if (writeToDB) batch = &localBatch;
	DatosUsuario datos = getDatos(uID);
	if (datos.fotoID == 0)
	{
		datos.fotoID = contadorActual(LAST_FID,"foto ID");
		incrementarContador(LAST_FID,"foto ID", batch);
	}
	// Creamos key y data para FID y THUMB
	IDKey keyFoto(FOTO, datos.fotoID);
	IDKey keyThumb(FOTO_THUMB, datos.fotoID);
	vector<char> bytesFoto(foto.toBytes());
	if (bytesFoto.size() > DBConstMaxJPGBytes)
		throw TooBigException("JPEG > 4 MB");
	vector<char> bytesThumb(foto.resize().toBytes());
	Slice fotoData(bytesFoto.data(), bytesFoto.size());
	Slice thumbData(bytesThumb.data(), bytesThumb.size());
	batch->Put(keyFoto.toSlice(), fotoData);
	batch->Put(keyThumb.toSlice(), thumbData);
	setDatos(uID, datos, batch, false);
	if (writeToDB) {
		Status status = db->Write(WriteOptions(), batch);
		verificarEstadoDB(status, "Error al guardar fotos");
	}

}

Foto DBRaw::getFoto(uint32_t uID) {
	DatosUsuario datos = getDatos(uID);
	string retVal;
	IDKey fotoKey(FOTO, datos.fotoID);
	Status status = db->Get(ReadOptions(), fotoKey.toSlice(), &retVal);
	verificarEstadoDB(status, "Error al obtener la foto del usuario");
	return Foto(retVal.data(), retVal.length());
}

Foto DBRaw::getFotoThumbnail(uint32_t uID) {
	DatosUsuario datos = getDatos(uID);
	string retVal;
	IDKey thumbKey(FOTO_THUMB, datos.fotoID);
	Status status = db->Get(ReadOptions(), thumbKey.toSlice(), &retVal);
	verificarEstadoDB(status, "Error al obtener el thumbnail de la foto del usuario");
	return Foto(retVal.data(), retVal.length());
}

void DBRaw::setPerfil(uint32_t uID, const DatosUsuario &datos,
		const vector<string> &skills, const vector<Puesto> &puestos,
			string *resumen, Foto *foto)
{
	WriteBatch batch;
	setDatos(uID, datos, &batch, true);
	setSkills(uID, skills, &batch, false);
	setPuestos(uID, puestos, &batch, false);
	if(resumen) setResumen(uID, *resumen, &batch, false);
	if(foto) setFoto(uID, *foto, &batch, false);
	db->Write(WriteOptions(), &batch);
}

uint32_t DBRaw::getPopularidad(uint32_t uID) {
	// TODO: Implementar
	return 0;
}


/*
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
}*

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



uint32_t DBRaw::numUltMensaje(uint32_t uID1, uint32_t uID2) {
}

std::vector<std::pair<uint32_t, string> > DBRaw::getMensajes(uint32_t uID1,
		uint32_t uID2, uint32_t numUltMensaje, uint32_t numPrimMensaje) {
}*/

uint16_t DBRaw::getNumContactos(uint32_t uID) {
	return 0; //TODO: Implementar
}

void DBRaw::verificarEstadoDB(Status status, const char *mensajeError, bool log)
{
	if (!status.ok()) {
		if (log) dbLog->errorStream() << mensajeError << ": " << status.ToString();
		throw LevelDBException(status.ToString());
	}
}

void DBRaw::inicializarFID(const string &rutaFotoDefault)
{
	if (!inicializarContador(LAST_FID, "foto ID")) return;
	std::ifstream fotoFile(rutaFotoDefault);
	if (!fotoFile.is_open()) throw std::ios_base::failure
			(string("No se encontro ").append(defaultFotoPath));
	string content = string(std::istreambuf_iterator<char>(fotoFile),
			std::istreambuf_iterator<char>());
	IDKey fotoKey(FOTO,0);
	IDKey thumbKey(FOTO_THUMB,0);
	Slice fotoSlice(content.data(), content.length());
	Slice thumbSlice = Foto(content.data(), content.length()).resize().toSlice();
	WriteOptions options;
	db->Put(options, fotoKey.toSlice(), fotoSlice);
	db->Put(options, thumbKey.toSlice(), thumbSlice);
	incrementarContador(LAST_FID, "foto ID");
}

bool DBRaw::inicializarContador(KeyCode keyCode, const string &tipo)
{
	try
	{
		contadorActual(keyCode, tipo, false);
		return false;
	}
	catch (LevelDBException &e)
	{
		char key = keyCode;
		uint32_t value = 0;
		Status status = db->Put(WriteOptions(), Slice(&key, 1), Slice((char*)&value, 4));
		string errorMsg(string("Error al inicializar ").append(tipo));
		verificarEstadoDB(status, errorMsg.c_str());
		return true;
	}
}

uint32_t DBRaw::contadorActual(KeyCode keyCode, const string &tipo, bool log)
{
	char key = keyCode;
	Slice lastIDKey(&key, 1);
	string retVal;
	Status status = db->Get(ReadOptions(), lastIDKey, &retVal);
	string errorMsg(string("Error al consultar contador de ").append(tipo));
	verificarEstadoDB(status, errorMsg.c_str(), log);
	uint32_t lastID;
	copy(retVal.begin(), retVal.end(), (char*) &lastID);
	return lastID;
}

void DBRaw::incrementarContador(KeyCode keyCode, const string &tipo, WriteBatch* batch)
{
	uint32_t proxUID = contadorActual(keyCode, tipo) + 1;
	char keyBytes = keyCode;
	Slice key(&keyBytes, 1);
	Slice value((char*) &proxUID, 4);
	if (batch) batch->Put(key, value);
	else
	{
		Status status = db->Put(WriteOptions(), key, value);
		string errorMsg(string("Error al incrementar").append(tipo));
		verificarEstadoDB(status, errorMsg.c_str(), log);
	}
}

template<class TException> void DBRaw::verificarContador(KeyCode keyCode,
		const string &tipo, uint32_t ID) {
	if (ID >= contadorActual(keyCode, tipo)) throw TException(std::to_string(ID));
	return;
}
