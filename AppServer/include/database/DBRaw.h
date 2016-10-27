#ifndef APPSERVER_INCLUDE_DATABASE_DBRAW_H_
#define APPSERVER_INCLUDE_DATABASE_DBRAW_H_

#include "../leveldb/db.h"
#include "../../include/leveldb/write_batch.h"
#include "../log4cpp/Appender.hh"
#include "../log4cpp/Category.hh"
#include "../handlers/SharedServerHandler.h"
#include "DBExceptions.h"
#include "DatosUsuario.h"
#include "Foto.h"
#include "Fecha.h"
#include "Geolocacion.h"
#include "Puesto.h"

using std::string;

// Foto por defecto
static const char* defaultFotoPath = "default.jpg";
enum KeyCode : uint8_t;

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

	/**
	 * Inicializa un contador en caso de no estar inicializado
	 * @param keyCode			Keycode que identifica el tipo de contador
	 * @param tipo				String que describe que es el contador para mensajes de error
	 * @return					True si fue necesario inicializar el contador
	 */
	bool inicializarContador(KeyCode keyCode, const string &tipo);

	/**
	 * Devuelve el proximo valor del contador
	 * @param keyCode			Keycode que identifica el tipo de contador
	 * @param tipo				String que describe que es el contador para mensajes de error
	 * @param log				Si se debe logear en caso de error
	 * @return					UID deseado
	 */
	uint32_t contadorActual(KeyCode keyCode, const string &tipo, bool log = true);

	/**
	 * Incrementa el registro de uIDs
	 * @param keyCode			Keycode que identifica el tipo de contador
	 * @param tipo				String que describe que es el contador para mensajes de error
	 * @param batch				Puntero si se quiere batchear el write, null sino
	 */
	void incrementarContador(KeyCode keyCode, const string &tipo,
			leveldb::WriteBatch* batch = NULL);

	/**
	 * Dado un codigo de estado de leveldb, arroja una excepcion si algo fallo
	 * @param status						Status devuelto por leveldb
	 * @param mensajeError					Contexto del error
	 * @param log							Si se debe logear el error en caso de existir
	 * @exception LevelDBException			Si algo fallo
	 */
	void verificarEstadoDB(leveldb::Status status, const char *mensajeError, bool log = true);

	/**
	 * Verifica si un uid existe, y sino lanza una excepcion
	 * @param keyCode					Keycode que identifica el tipo de contador
	 * @param tipo						String que describe que es el contador para mensajes de error
	 * @param uID						El ID
	 */
	template<class T> void verificarContador(KeyCode keyCode, const string &tipo, uint32_t ID);

	/**
	 * Inicializa el foto ID si no existe
	 * Carga la foto default si es necesario
	 * @param rutaFotoDefault			Ruta a un jpg
	 */
	void inicializarFID(const string &rutaFotoDefault);

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
	 * @param datos						Datos varios del usuario
	 * @param userName					Nombre de usuario para login
	 * @param passHash					SHA-256 de la password del usuario
	 * @exception PreexistentUsername	Nombre de usuario ya tomado
	 * @return							El uID (user ID) del nuevo usuario creado
	 */
	uint32_t registrarse(const DatosUsuario &datos, const string &userName,
			const std::vector<char> &passHash);

	/**
	 * Agrega una nueva foto al usuario de la uID pedida
	 * Se espera que la foto este en formato jpg
	 * En caso de que ya existiera una foto para este usuario, la reemplaza
	 * @param uID						User ID del usuario
	 * @param foto						Foto
	 * @param batch						Null para escribir a db, puntero para batchear
	 * @param verifUID					Si debe realizarse la verificacion sobre el UID
	 * @exception NonexistentUserID		El uID no es válido
	 * @exception BadInputException		La foto no es un jpg
	 */
	void setFoto(uint32_t uID, const Foto &foto,
			leveldb::WriteBatch *batch = NULL, bool verifUID = true);

	/**
	 * Actualiza el resumen profesional del usuario
	 * @param uID						User ID
	 * @param resumen					El resumen personal arbitrario que escribio el usuario
	 * @param batch						Null para escribir a db, puntero para batchear
	 * @param verifUID					Si debe realizarse la verificacion sobre el UID
	 * @exception NonexistentUserID		El uID no es válido
	 */
	void setResumen(uint32_t uID, const string &resumen,
			leveldb::WriteBatch *batch = NULL, bool verifUID = true);

	/**
	 * Devuelve el resumen profesional del usuario
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 */
	string getResumen(uint32_t uID);

	/**
	 * Devuelve todos los datos de usuario que el sistema tiene,
	 * salvo por la foto y resumen
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Datos
	 */
	DatosUsuario getDatos(uint32_t uID);

	/**
	 * Actualiza todos los datos de usuario que el sistema tiene,
	 * salvo foto y resumen
	 * @param uID						UserID
	 * @param datos						Datos
	 * @param batch						Null para escribir a db, puntero para batchear
	 * @param verifUID					Si debe realizarse la verificacion sobre el UID
	 * @exception NonexistentUserID		El uID no es válido
	 */
	void setDatos(uint32_t uID, const DatosUsuario &datos,
			leveldb::WriteBatch *batch = NULL, bool verifUID = true);

	/**
	 * Devuelve la foto del usuario
	 * En caso de no existir devuelve una por defecto
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							TODO: Ver retorno
	 */
	Foto getFoto(uint32_t uID);

	/**
	 * Devuelve el thumbnail (64x64) de la foto del usuario
	 * En caso de no existir devuelve uno por defecto
	 * @param uID			User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							TODO: Ver retorno
	 */
	Foto getFotoThumbnail(uint32_t uID);

	void setSkills(uint32_t uID, std::vector<string> skills,
			leveldb::WriteBatch *batch = NULL, bool verifUID = true);
	std::vector<string> getSkills(uint32_t uID);

	/**
	 * @param uID						User ID
	 * @param puestos					Vecotr de puestos
	 * @param batch						Null para escribir a db, puntero para batchear
	 * @param verifUID					Si debe realizarse la verificacion sobre el UID
	 * @exception NonexistentUserID		El uID es inválido
	 */
	void setPuestos(uint32_t uID, std::vector<Puesto> puestos,
			leveldb::WriteBatch *batch = NULL, bool verifUID = true);

	/**
	 * Devuelve la lista de puestos de un usuario
	 * @param uID
	 * @return
	 */
	std::vector<Puesto> getPuestos(uint32_t uID);

	/**
	 *  Hace login y devuelve un uid válido si la info es correcta
	 *  o lanza una excepcion si no lo es
	 * @param userName					Nombre de usuario
	 * @param passHash					Hash de la password del usuario de 256 bit
	 * @return							El user ID
	 * @exception NonexistentUsername	Datos de login incorrectos por username inexistente
	 * @exception BadPassword			Datos de login incorrectos por password incorrecto
	 */
	uint32_t login(const string &userName, const std::vector<char> &passHash);

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
	std::vector<uint32_t> busquedaProfresional(const std::vector<string>
		*puestos, const std::vector<string> *skill, const std::vector<string>
		*categorias, Geolocacion *geolocacion, float maxDist,
		bool sortPopularidad);

	/**
	 * Realiza una busqueda sobre solo los usuarios más populares
	 * @param conteo					Cuantos de los más populares retornar
	 * @return							Vector con todos los uIDs matcheantes
	 */
	std::vector<uint32_t> busquedaPopular (uint conteo = 10);

	/**
	 * Realiza una busqueda sobre solo los usuarios más populares, que posean un skill
	 * @param conteo					Cuantos de los más populares retornar
	 * @param skill						Nombre del skill
	 * @return							Vector con todos los uIDs matcheantes
	 */
	std::vector<uint32_t> busquedaPopularSkill (const string &skill, uint conteo = 10);

	/**
	 * Realiza una busqueda sobre solo los usuarios más populares, que posean un puesto
	 * @param conteo					Cuantos de los más populares retornar
	 * @return							Vector con todos los uIDs matcheantes
	 */
	std::vector<uint32_t> busquedaPopularPuesto (const string &puesto, uint conteo = 10);

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
	 * @exception NonexistentRequest	No existe peticion entre estos usuarios
	 * @return							El mensaje
	 */
	string getMsgSolicitud(uint32_t uIDFuente, uint32_t uIDDestino);

	/**
	 * Elimina la solicitud de contacto y vuelve a ambos contactos uno del otro
	 * @param uIDFuente					User ID de quien pide agregar contacto
	 * @param uIDDestino				User ID de quien será agregado
	 * @exception NonexistentRequest	No existe peticion entre estos usuarios
	 */
	void aceptarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino);

	/**
	 * Elimina la solicitud de contacto
	 * @param uIDFuente					User ID de quien pide agregar contacto
	 * @param uIDDestino				User ID de quien será agregado
	 * @exception NonexistentRequest	No existe peticion entre estos usuarios
	 */
	void eliminarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino);

	/**
	 * Elimina el vinculo de contacto entre dos usuarios
	 * @param uID1						User ID 1
	 * @param uID2						User ID 2
	 * @exception NonexistentContact	No eran contactos
	 */
	void eliminarContacto(uint32_t uID1, uint32_t uID2);

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
