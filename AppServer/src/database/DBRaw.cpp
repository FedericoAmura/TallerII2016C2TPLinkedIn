#include "../../include/database/DBRaw.h"
#include "../../include/database/DBExceptions.h"
#include "../../include/database/DBConstants.h"
#include "../../include/log4cpp/OstreamAppender.hh"
#include "../../include/log4cpp/BasicLayout.hh"
#include <fstream>
#include <algorithm>

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
	SOLIC_COUNT,
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
 IDKey(KeyCode prefijo, uint32_t key1, uint32_t key2) : data(9) {
 	 data[0] = prefijo;
 	 copy((char*)&key1, (char*)(&key1+1), data.begin()+1);
 	 copy((char*)(&key2), (char*)(&key2+1), data.begin()+5);
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
}*/

void DBRaw::solicitarContacto(uint32_t uIDFuente, uint32_t uIDDestino,
		const string& mensaje) {
	if (uIDFuente == uIDDestino) throw AlreadyContacts("Contacto con si mismo");
	// No hace falta LOCK porque no es relevante si si pisa la misma solicitud
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uIDFuente);
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uIDDestino);
	// Verificamos si ya son contactos, y en ese caso no hago nada
	vector<uint32_t> contactos = getContactos(uIDDestino);
	if (std::find(contactos.begin(), contactos.end(), uIDFuente) != contactos.end())
		throw AlreadyContacts("Ya eran contactos");
	WriteBatch batch;
	// Si ya existe la solicitud, se la reescribe
	vector<uint32_t> pending = getSolicitudes(uIDDestino);
	bool existe = std::find(pending.begin(), pending.end(), uIDFuente) != pending.end();
	if (!existe)	{
		pending.push_back(uIDFuente);
	}
	uint16_t contador = pending.size();
	IDKey solicKey(SOLIC, uIDDestino);
	IDKey solicTextKey(SOLIC_TXT, uIDFuente, uIDDestino);
	IDKey solicCountKey(SOLIC_COUNT, uIDDestino);
	Slice solicData((char*)pending.data(), pending.size()*sizeof(uint32_t));
	Slice msgData(mensaje.data(), mensaje.length());
	Slice countData((char*) &contador, sizeof(uint16_t));
	batch.Put(solicKey.toSlice(), solicData);
	batch.Put(solicTextKey.toSlice(), msgData);
	batch.Put(solicCountKey.toSlice(), countData);
	Status status = db->Write(WriteOptions(), &batch);
	verificarEstadoDB(status, "Error al guardar nueva solicitud de contacto.");
}

vector<uint32_t> DBRaw::getSolicitudes(uint32_t uIDConsultador) {
	vector<uint32_t> result;
	string retVal;
	IDKey solicKey(SOLIC, uIDConsultador);
	Status status = db->Get(ReadOptions(), solicKey.toSlice(), &retVal);
	if (!status.IsNotFound())
		verificarEstadoDB(status, "Error al obtener solicitudes.");
	for (int i = 0; i < retVal.length(); i += sizeof(uint32_t))
	{
		result.push_back(retVal.data()[i]);
	}
	return result;
}

uint16_t DBRaw::getNumSolicitudes(uint32_t uID) {
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	string retVal;
	IDKey countKey(SOLIC_COUNT, uID);
	Status status = db->Get(ReadOptions(), countKey.toSlice(), &retVal);
	if (status.IsNotFound())
		return 0;
	verificarEstadoDB(status, "Error al obtener numero de solicitudes.");
	return uint16_t(retVal.data()[0]);
}

string DBRaw::getMsgSolicitud(uint32_t uIDFuente, uint32_t uIDDestino) {
	string retVal;
	IDKey solicTextKey(SOLIC_TXT, uIDFuente, uIDDestino);
	Status status = db->Get(ReadOptions(), solicTextKey.toSlice(), &retVal);
	if (status.IsNotFound())
		throw NonexistentRequest("No existe el pedido de contacto");
	verificarEstadoDB(status, "Error al obtener mensaje de solicitud.");
	return retVal;
}

void DBRaw::aceptarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino) {
	WriteBatch batch;
	// TODO: Lock para ambos users
	/**
	 * Se supone que al crear la solicitud se chequea si ya eran contactos
	 * y ahora no hace falta
	 */
	getMsgSolicitud(uIDFuente, uIDDestino); // Chequeo que exista la peticion
	uint32_t uIDMenor = uIDFuente < uIDDestino ? uIDFuente : uIDDestino;
	uint32_t uIDMayor = uIDFuente >= uIDDestino ? uIDFuente : uIDDestino;
	vector<uint32_t> contactosFuente = getContactos(uIDFuente);
	vector<uint32_t> contactosDestino = getContactos(uIDDestino);
	contactosFuente.push_back(uIDDestino);
	contactosDestino.push_back(uIDFuente);
	uint16_t newCountFuente = contactosFuente.size();
	uint16_t newCountDest = contactosDestino.size();

	IDKey contactFuenteKey(US_CONTACT, uIDFuente);
	IDKey contactDestKey(US_CONTACT, uIDDestino);
	IDKey countFuenteKey(US_CONTACT_COUNT, uIDFuente);
	IDKey CountDestKey(US_CONTACT_COUNT, uIDDestino);
	IDKey pairKey(US_CONTACT_PAIR, uIDMenor, uIDMayor);

	Slice contactFuenteData((char*) contactosFuente.data(),
			contactosFuente.size()*sizeof(uint32_t));
	Slice contactDestData((char*) contactosDestino.data(),
			contactosDestino.size()*sizeof(uint32_t));
	Slice countFuenteData((char*)&newCountFuente, sizeof(uint16_t));
	Slice countDestData((char*)&newCountDest, sizeof(uint16_t));
	Slice pairData;
	batch.Put(contactFuenteKey.toSlice(), contactFuenteData);
	batch.Put(contactDestKey.toSlice(), contactDestData);
	batch.Put(countFuenteKey.toSlice(), countFuenteData);
	batch.Put(CountDestKey.toSlice(), countDestData);
	batch.Put(pairKey.toSlice(), pairData);
	eliminarSolicitud(uIDFuente, uIDDestino, &batch);
	Status status = db->Write(WriteOptions(), &batch);
	verificarEstadoDB(status, "Error al aceptar solicitud de contacto.");
}

void DBRaw::eliminarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino,
		WriteBatch *batch) {
	WriteBatch localBatch;
	bool writeToDB = !batch;
	if (writeToDB) batch = &localBatch;
	vector<uint32_t> pending = getSolicitudes(uIDDestino);
	vector<uint32_t>::iterator it = std::find(pending.begin(), pending.end(), uIDFuente);
	if (it == pending.end()) throw NonexistentRequest("No existe el pedido de contacto");
	pending.erase(it);
	uint16_t contador = pending.size();
	IDKey solicKey(SOLIC, uIDDestino);
	IDKey solicTextKey(SOLIC_TXT, uIDFuente, uIDDestino);
	IDKey solicCountKey(SOLIC_COUNT, uIDDestino);
	Slice solicData((char*)pending.data(), pending.size()*sizeof(uint32_t));
	Slice solicCountData((char*)&contador, sizeof(uint16_t));
	batch->Put(solicKey.toSlice(), solicData);
	batch->Put(solicCountKey.toSlice(), solicCountData);
	batch->Delete(solicTextKey.toSlice());
	if (writeToDB)
	{
		Status status = db->Write(WriteOptions(), batch);
		if (status.IsNotFound())
			throw NonexistentRequest("No existe el pedido de contacto");
		verificarEstadoDB(status, "Error al eliminar solicitud de contacto.");
	}
}

/**
 * Elimina el opuesto de cada lista de contactos por uid
 * Elimina el par
 * Reduce el contador de cada uid
 */
void DBRaw::eliminarContacto(uint32_t uID1, uint32_t uID2) {
	WriteBatch batch;
	uint32_t uIDMenor = uID1 < uID2 ? uID1 : uID2;
	uint32_t uIDMayor = uID1 >= uID2 ? uID1 : uID2;
	vector<uint32_t> contactos1 = getContactos(uID1);
	vector<uint32_t> contactos2 = getContactos(uID2);
	vector<uint32_t>::iterator it = std::find(contactos1.begin(), contactos1.end(), uID2);
	if (it == contactos1.end()) throw NonexistentContact("No eran contactos");
	contactos1.erase(it);
	it = std::find(contactos2.begin(), contactos2.end(), uID1);
	contactos2.erase(it);
	uint16_t newCount1 = contactos1.size();
	uint16_t newCount2 = contactos2.size();

	IDKey contact1Key(US_CONTACT, uID1);
	IDKey contact2Key(US_CONTACT, uID2);
	IDKey count1Key(US_CONTACT_COUNT, uID1);
	IDKey count2Key(US_CONTACT_COUNT, uID2);
	IDKey pairKey(US_CONTACT_PAIR, uIDMenor, uIDMayor);

	Slice contact1Data((char*) contactos1.data(), contactos1.size()*sizeof(uint32_t));
	Slice contact2Data((char*) contactos2.data(), contactos2.size()*sizeof(uint32_t));
	Slice count1Data((char*)&newCount1, sizeof(uint16_t));
	Slice count2Data((char*)&newCount2, sizeof(uint16_t));

	batch.Put(contact1Key.toSlice(), contact1Data);
	batch.Put(contact2Key.toSlice(), contact2Data);
	batch.Put(count1Key.toSlice(), count1Data);
	batch.Put(count2Key.toSlice(), count2Data);
	batch.Delete(pairKey.toSlice());
	Status status = db->Write(WriteOptions(), &batch);
	verificarEstadoDB(status, "Error al eliminar contacto.");
}

std::vector<uint32_t> DBRaw::getContactos(uint32_t uID) {
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	vector<uint32_t> result;
	string retVal;
	IDKey solicKey(US_CONTACT, uID);
	Status status = db->Get(ReadOptions(), solicKey.toSlice(), &retVal);
	if (!status.IsNotFound())
		verificarEstadoDB(status, "Error al obtener solicitudes.");
	for (int i = 0; i < retVal.length(); i += sizeof(uint32_t))
	{
		result.push_back(retVal.data()[i]);
	}
	return result;
}

uint16_t DBRaw::getNumContactos(uint32_t uID) {
	verificarContador<NonexistentUserID>(LAST_UID, string("user ID"), uID);
	string retVal;
	IDKey countKey(US_CONTACT_COUNT, uID);
	Status status = db->Get(ReadOptions(), countKey.toSlice(), &retVal);
	if (status.IsNotFound())
		return 0;
	verificarEstadoDB(status, "Error al obtener numero de contactos.");
	return uint16_t(retVal.data()[0]);
}

bool DBRaw::sonContactos(uint32_t uID1, uint32_t uID2) {
	uint32_t uIDMenor = uID1 < uID2 ? uID1 : uID2;
	uint32_t uIDMayor = uID1 >= uID2 ? uID1 : uID2;
	IDKey pairKey(US_CONTACT_PAIR, uIDMenor, uIDMayor);
	string retVal;
	Status status = db->Get(ReadOptions(), pairKey.toSlice(), &retVal);
	if (status.IsNotFound()) return false;
	verificarEstadoDB(status, "Error al obtener numero de contactos.");
	return true;
}


/*
uint32_t DBRaw::numUltMensaje(uint32_t uID1, uint32_t uID2) {
}

std::vector<std::pair<uint32_t, string> > DBRaw::getMensajes(uint32_t uID1,
		uint32_t uID2, uint32_t numUltMensaje, uint32_t numPrimMensaje) {
}*/

/**
 * Metodos privados ------------------------------------------------
 */

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
