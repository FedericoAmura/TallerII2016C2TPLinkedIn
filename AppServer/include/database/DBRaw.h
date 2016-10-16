#ifndef APPSERVER_INCLUDE_DATABASE_DBRAW_H_
#define APPSERVER_INCLUDE_DATABASE_DBRAW_H_

#include "../leveldb/db.h"
#include "../../include/log4cpp/Appender.hh"
#include "../include/log4cpp/Category.hh"

using std::string;

/**
 * Clase para representar fechas de una forma que permita una facil transicion entre strings
 * Y el tipo compacto en 4 bytes
 * Las fechas como string seran en formato AAAA/MM/DD
 * Para fecha actual/infinito se tomara el string "current"
 * Las fechas uint32_t son en el siguiente formato:
 * 32 bits = 16 bits año, 8 bits mes, 8 bits día
 * En binario: AAAAAAAA AAAAAAAA MMMMMMMM DDDDDDDD
 * Para fecha actual/infinito se tomara la fecha 0xffffffff
 */
class Fecha
{
 public:
	Fecha(const string &fecha);
	Fecha(uint32_t fecha);
	string toString();
	uint32_t toUint32_t();
 private:
	uint32_t fecha;
};

/**
 * Clase que encapsula un puesto de trabajo y un periodo de tiempo
 */
class Puesto
{
 public:
	/**
	 * @param puesto						Nombre del puesto
	 * @param fechaInicio					Fecha de inicio
	 * @param fechaFin						Fecha de fin
	 */
	Puesto(const string &puesto, Fecha fechaInicio, Fecha fechaFin);
	string puesto;
	Fecha fechaInicio;
	Fecha fechaFin;
};

class Geolocacion
{
public:
	/**
	 * Constructor
	 * @param longitud
	 * @param latitud
	 * @exception BadGeolocation		Numeros fuera de rango
	 */
	Geolocacion(double longitud, double latitud);
	double longitud();
	double latitud();
private:
	double _longitud;
	double _latitud;
};

/**
 * Clase que representa varios de los datos de usuario agrupados
 */
class Datos
{
	/**
	 * Constructor por parametro
	 * @param nombre					Nombre y apellido de la persona
	 * @param email						Dirección de email
	 * @param fechaNacimiento			Fecha de nacimiento segun el formato establecido
	 * @param userName					Nombre de usuario
	 * @param passHash					SHA-256 de la password del usuario
	 * @param ciudad					Ciudad donde vive
	 * @param longitud					Longitud geográfica
	 * @param latitud					Latitud geográfica
	 */
	Datos(const string &nombre, const string &email, Fecha fechaNacimiento,
			const string &userName, const uint8_t *passHash, const string &ciudad,
			Geolocacion geolocacion);
	string nombre;
	string email;
	Fecha fechaNacimiento;
	string userName;
	uint8_t passHash;
	string ciudad;
	Geolocacion geolocacion;
};

/**
 * Clase que representa la información de una foto
 */
class Foto
{
 public:
	Foto(const string &datosBase64);
	Foto(const uint8_t* datosBinarios);
	string toBase64String();
	const uint32_t* toUint32_t();
 private:
	uint8_t* datosBinarios;
};

/**
 * Clase wrapper de levelDB con el modelo de datos propio de la aplicación
 * Maneja tipos de datos std o propios y devuelve tipos de datos std o propios
 * Si un input es mayor al tamaño especificado, en caso de strings se los truncara
 * En caso de datos binarios se levantara una excepción
 */
class DBRaw {
 private:
	leveldb::DB* db;
	log4cpp::Appender *dbLogAppender;
	log4cpp::Category *dbLog;
	std::ostream *logStream;
	SharedServerHandler* sharedServerHandler;
 public:
	/**
	 * Constructor
	 * @param rutaArchivo				Ruta al archivo de base de datos, si no existe se crea
	 * @param logStream					Stream al cual logear, se elimina al eliminarse
	 * 									El logstream se autoelimina al eliminar esta instancia
	 * @exception LevelDBException		Si falla algo al abrir la base de datos
	 */
	DBRaw(const string &rutaArchivo, std::ostream *logStream);

	virtual ~DBRaw();

	/**
	 * Registrar un nuevo usuario
	 * @exception PreexistentUsername	Nombre de usuario ya tomado
	 * @return							El uID (user ID) del nuevo usuario creado
	 */
	uint32_t registrarse(const Datos &datos);

	/**
	 * Agrega una nueva foto al usuario de la uID pedida
	 * Se espera que la foto este en formato jpg
	 * En caso de que ya existiera una foto para este usuario, la reemplaza
	 * @param uID						User ID del usuario
	 * @param foto						Foto
	 * @exception NonexistentUserID		El uID no es válido
	 * @exception BadInputException		La foto no es un jpg
	 */
	void actualizarFoto(uint32_t uID, Foto &foto);

	/**
	 * Agrega un skill a un usuario
	 * @param uID						User ID
	 * @param skill						Nombre del skill
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception NonexistentSkill		El skill es inválido (no existe en el shared server)
	 */
	void agregarSkill(uint32_t uID, const string &skill);

	/**
	 * Elimina un skill del usuario
	 * @param uID						User ID
	 * @param skill						Nombre del Skill
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception NonexistentSkill		El skill es inválido (no existe en el usuario)
	 */
	void eliminarSkill(uint32_t uID, const string &skill);

	/**
	 * Reemplaza todos los skills del usuario por otros
	 * Realiza la comparacion necesaria para no realizar operaciones innecesarias
	 * @param uID						User ID
	 * @param skills					Lista de nombres de skills
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception NonexistentSkill		Alguno de los skills es inválido (no existe en el shared server)
	 */
	void actualizarSkills(uint32_t uID, const std::vector<string> &skills);

	/**
	 * Agrega un puesto de trabajo a los datos de un usuario
	 * Las fechas deben venir en el formato establecido
	 * Se chequeara que la fecha sea valida siempre
	 * @param uID							User ID
	 * @param puesto						El puesto con inicio y fin
	 * @exception NonexistentUserID			El uID es inválido
	 * @exception NonexistentPosition		El nombre del puesto no es válido (no existe en el shared server)
	 * @exception MalformedDate				Las fechas son de alguna forma invalidas
	 */
	void agregarPuesto(uint32_t uID, const Puesto &puesto);

	/**
	 * Elimina la posición pedida al usuario del dado uID
	 * Se piden las fechas porque el usuario puede haber tenido el mismo
	 * trabajo más de una vez, y no hay forma de identificarlo sino
	 * @param uID							User ID
	 * @param puesto						El puesto con inicio y fin
	 * @exception NonexistentUserID			El uID es inválido
	 * @exception NonexistentPosition		El puesto no es válido (no existe en el usuario)
	 * @exception NonexistentDate 			Las fechas no concuerdan con las del usuario
	 */
	void eliminarPuesto(uint32_t uID, const Puesto &puesto);

	/**
	 * Reemplaza todos los puestos del usuario por otros
	 * Realiza la comparacion necesaria para no realizar operaciones innecesarias
	 * @param uID							User ID
	 * @param puesto						Los puestos con su inicio y fin
	 * @exception NonexistentUserID			El uID es inválido
	 * @exception NonexistentPosition		Algun nombre del puesto no es válido (no existe en el shared server)
	 */
	void actualizarPuestos(uint32_t uID, const std::vector<Puesto> puestos);

	/**
	 * Actualiza el mail del usuario del uID dado
	 * TODO: Hallar la manera de verificar si un mail existe
	 * @param uID						User ID
	 * @param email						El email
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception NonexistentEmail		El email no es un mail valido
	 */
	void actualizarEMail(uint32_t uID, const string &email);

	/**
	 * Actualiza la ciudad del usuario del uID dado
	 * No se chequea que la ciudad en cuestión realmente exista
	 * @param uID						User ID
	 * @param ciudad					Nombre de la ciudad
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception NonexistentEmail		El email no es un mail valido
	 */
	void actualizarCiudad(uint32_t uID, const string &ciudad);

	/**
	 * Actualiza la locación geografica del usuario a la dada
	 * @param uID						User ID
	 * @param geolocacion				Locación geográfica
	 * @exception NonexistentUserID		El uID es inválido
	 */
	void actualizarGeolocacion(uint32_t uID, Geolocacion geolocacion);

	/**
	 * Actualiza el resumen profesional del usuario
	 * @param uID						User ID
	 * @param resumen					El resumen personal arbitrario que escribio el usuario
	 * @exception NonexistentUserID		El uID es inválido
	 */
	void actualizarResumen(uint32_t uID, const string &resumen);

	/**
	 * Devuelve todos los datos de usuario que el sistema tiene,
	 * salvo por la foto y resumen
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Datos
	 */
	Datos getDatos(uint32_t uID);

	/**
	 * Actualiza todos los datos de usuario que el sistema tiene,
	 * salvo foto y resumen
	 * @exception NonexistentUserID		El uID no es válido
	 */
	void actualizarDatos(uint32_t uID, const Datos &datos);

	/**
	 * Devuelve la foto del usuario
	 * En caso de no existir devuelve una por defecto
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							TODO: Ver retorno
	 */
	Foto getFoto(uint32_t uID);

	/**
	 * Devuelve el thumbnail de 128x128 de la foto del usuario
	 * En caso de no existir devuelve uno por defecto
	 * @param uID			User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							TODO: Ver retorno
	 */
	Foto getFotoThumbnail(uint32_t uID);

	/**
	 *  Hace login y devuelve un uid válido si la info es correcta
	 *  o lanza una excepcion si no lo es
	 * @param username					Nombre de usuario
	 * @param passwordHash				Hash de la password del usuario de 256 bit
	 * @return							El user ID
	 * @exception NonexistentUsername	Datos de login incorrectos por username inexistente
	 * @exception BadPassword			Datos de login incorrectos por password incorrecto
	 */
	uint32_t login(const string &username, const uint8_t* passwordHash);

	/**
	 * Realiza una búsqueda sobre los usuarios con los parametros dados.
	 * Si se pasa una categoria, posiblemente los vectores de skills y puestos
	 * sean modificados
	 * @param puestos					Lista de puestos. Null si no aplica.
	 * @param skill						Lista de skills. Null si no aplica.
	 * @param categorias				Lista de categorias. Null si no aplica.
	 * @param geolocacion				Punto geográfico. Null si no aplica.
	 * @param maxDist					Máxima distancia desde el punto. Infinito vale.
	 * @param sortPopularidad			Si se debe ordernar los resultados por popularidad.
	 * @return							Vector con todos los uIDs matcheantes
	 * @exception NonexistentSkill		Skill invalido
	 * @exception NonexistentPosition	Puesto invalido
	 * @exception NonexistentCategory	Categoria invalida
	 * @exception BadInputException		Distancia máxima negativa
	 */
	std::vector<uint32_t> busqueda_profresional(const std::vector<string>
		*puestos, const std::vector<string> *skill, const std::vector<string>
		*categorias, Geolocacion *geolocacion, float maxDist,
		bool sortPopularidad);

	/**
	 * Realiza una busqueda sobre solo los usuarios más populares
	 * @param conteo					Cuantos de los más populares se usaran para la busqueda
	 * @param puestos					Lista de puestos. Null si no aplica.
	 * @param skill						Lista de skills. Null si no aplica.
	 * @param categorias				Lista de categorias. Null si no aplica.
	 * @exception NonexistentSkill		Alguno de los skills es invalido
	 * @exception NonexistentPosition	Alguno de los puestos es invalido
	 * @exception NonexistentCategory	Alguna de las categorias es invalida
	 * @return							Vector con todos los uIDs matcheantes
	 */
	std::vector<uint32_t> busqueda_popularidad (uint conteo,
			const std::vector<string> *puestos, const std::vector<string>
			*skill, const std::vector<string> *categorias);

	/**
	 * Envia un pedido de contacto
	 * @param uIDFuente					User ID de quien pide agregar contacto
	 * @param uIDDestino				User ID de quien será agregado
	 * @param mensaje					Mensaje definido por el usuario que envia la solicitud
	 * @exception NonexistentUserID		Alguno de los uIDs es inválido
	 */
	void solicitarContacto(uint32_t uIDFuente, uint32_t uIDDestino, const string &mensaje);

	/**
	 * Devuelve la lista de user IDs que quieren ser contactos del consultador
	 * @param uIDConsultador			User ID de quien busca la información
	 * @exception NonexistentUserID		El uID parametro es inválido
	 * @return							Vector de IDs
	 */
	std::vector<uint32_t> getSolicitudes(uint32_t uIDConsultador);

	/**
	 * Devuelve el mensaje de una solicitud particular
	 * @param uIDFuente					User ID de quien pide agregar contacto
	 * @param uIDDestino				User ID de quien será agregado
	 * @exception NonexistentUserID		Alguno de los uIDs es inválido
	 * @exception NonexistentRequest	No existe la peticion
	 * @return							El mensaje
	 */
	string getMsgSolicitud(uint32_t uIDFuente, uint32_t uIDDestino);

	/**
	 * Elimina la solicitud de contacto y vuelve a ambos contactos uno del otro
	 * @param uIDFuente					User ID de quien pide agregar contacto
	 * @param uIDDestino				User ID de quien será agregado
	 * @exception NonexistentUserID		Alguno de los uIDs es inválido
	 * @exception NonexistentRequest	No existe la peticion
	 */
	void aceptarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino);

	/**
	 * Elimina la solicitud de contacto
	 * @param uIDFuente					User ID de quien pide agregar contacto
	 * @param uIDDestino				User ID de quien será agregado
	 * @exception NonexistentUserID		Alguno de los uIDs es inválido
	 * @exception NonexistentRequest	No existe la peticion
	 */
	void eliminarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino);

	/**
	 * Devuelve la lista de contactos del usuario
	 * @param uID						User ID
	 * @exception NonexistentUserID		UserID inválido
	 * @return							Vector de user IDs
	 */
	std::vector<uint32_t> getContactos(uint32_t uID);

	/**
	 * Devuelve el numero de contactos del usuario
	 * @param uID						User ID
	 * @return							Número de contactos
	 * @exception NonexistentUserID		UserID inválido
	 */
	uint getNumContactos(uint32_t uID);

	/**
	 * Devuelve el numero del ultimo mensaje en una conversacion
	 * El orden de los uIDs es irrelevante
	 * @param uID1			User ID1 de la conversación
	 * @param uID2			User ID2 de la conversación
	 * @return				Número del último mensaje
	 * @exception			UserIDs inválidos
	 */
	uint32_t numUltMensaje(uint32_t uID1, uint32_t uID2);

	/**
	 * Devuelve los mensajes comprendidos entre #ultmensaje y #primmensaje
	 * Si estan fuera de rango no los devuelve
	 * El orden de los uIDs es irrelevant
	 * @param uID1				User ID1 de la conversación
	 * @param uID2				User ID2 de la conversación
	 * @param numUltMensaje		#Ultimo mensaje
	 * @param numPrimMensaje	#Primer mensaje
	 * @return					Vector de tuplas<uIDsender,mensaje>
	 */
	std::vector<std::pair<uint32_t,string> > getMensajes(uint32_t uID1,
			uint32_t uID2, uint32_t numUltMensaje, uint32_t numPrimMensaje);
};

#endif  /* APPSERVER_INCLUDE_DATABASE_DBRAW_H_ */
