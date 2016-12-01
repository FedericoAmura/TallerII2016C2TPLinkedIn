#ifndef APPSERVER_INCLUDE_DATABASE_DBRAW_H_
#define APPSERVER_INCLUDE_DATABASE_DBRAW_H_

#include "../leveldb/db.h"
#include "../leveldb/write_batch.h"
#include "DBExceptions.h"
#include "DatosUsuario.h"
#include "Foto.h"
#include "Fecha.h"
#include "Geolocacion.h"
#include "Puesto.h"
#include "DBConstants.h"
#include "../common/Logger.h"

using std::string;
using std::vector;
using leveldb::WriteBatch;

// Foto por defecto
static const char* defaultFotoPath = "default.jpg";
enum KeyCode : uint8_t;

/**
 * Clase wrapper de levelDB con el modelo de datos propio de la aplicación
 * Maneja tipos de datos std o propios y devuelve tipos de datos std o propios
 * Si un input es mayor al tamaño especificado, en caso de strings se los truncara
 * En caso de datos binarios se levantara una excepción
 * No exta anotado en cada metodo, pero todos pueden arrojar LevelDBException si la BD
 * Falla de alguna forma inesperada
 */
class DBRaw {
 private:
	leveldb::DB* db;

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
			WriteBatch* batch = NULL);

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

	/**
	 * Dada una key devuelve el vector de UIDs guardado en la DB
	 * Bajo esa key
	 * @param key						Key
	 * @param errorMsg					Mensaje de error a mostrar en caso de error
	 * @exception NonexistentKey		No existe la key
	 * @return
	 */
	vector<uint32_t> getUIDVector(const leveldb::Slice &key, const string &errorMsg);

	/**
	 * Dada una key devuelve el vector el uint32_t guardado en la DB
	 * Bajo esa key
	 * @param key						Key
	 * @param errorMsg					Mensaje de error a mostrar en caso de error
	 * @exception NonexistentKey		No existe la key
	 * @return
	 */
	uint32_t getUint(const leveldb::Slice &key, const string &errorMsg);

	/**
	 * Dada una key devuelve si existe o no en la DB
	 * @param key						Key
	 * @param errorMsg					Mensaje de error a mostrar en caso de error de DB
	 * @return
	 */
	bool getKeyExists(const leveldb::Slice &key, const string &errorMsg);

	/**
	 * Dado un Slice que es key de un array de uint32_t
	 * Eliminar el uint32_t del mismo si existe y vuelve a
	 * guardar el vector con la key dada en el batch
	 * @param userID				User ID a eliminar
	 * @param key					Key al array en db
	 * @param batch					Batch a usar
	 */
	void eraseVectorUID(uint32_t userID, const leveldb::Slice &key,
			WriteBatch &batch);

	/**
	 * Dado un Slice que es key de un array de uint32_t
	 * Agrega el uint32_t al vector (no chequea no repetirse)
	 * y vuelve a guardar el vector con la key dada en el batch
	 * @param userID				User ID a agregar
	 * @param key					Key al array en db
	 * @param batch					Batch a usar
	 */
	void orderedAppendVectorUID(uint32_t userID, const leveldb::Slice &key,
			WriteBatch &batch);

	/**
	 * Actualiza la locacion geografica de un usuario para reverse lookups
	 * @param uID					User ID
	 * @param viejaLocacion			Vieja locacion
	 * @param nuevaLocacion			Nueva locacion
	 * @param batch					Batch al cual escribir
	 */
	void rlGeolocacionUpdate(uint32_t uID, Geolocacion viejaLocacion,
			Geolocacion nuevaLocacion, WriteBatch &batch);

	/**
	 * Actualiza la entrada de la db utilizada para lookups de popularidad
	 * @param uID						User ID
	 * @param newPop					Nuevo valor de popularidad
	 * @param batch						Batch al cual escribir
	 */
	void rlPopularidadUpdate(uint32_t uID, uint32_t newPop, WriteBatch &batch);

	/**
	 * Actualiza la entrada de la db utilizada para lookups de strings
	 * @param userID			User ID
	 * @param code				KeyCode de key de DB
	 * @param batch				Batch a usar
	 * @param viejos			Strings viejos (los de la DB)
	 * @param nuevos			Strings nuevos del usuario que reeemplazarn a los viejos
	 */
	void rlUpdate(uint32_t userID, KeyCode code, WriteBatch &batch,
			vector<string> viejos, vector<string> nuevos);

	/**
	 * Actualiza la entrada de la db utilizada para lookups de puestos
	 * @param userID			User ID
	 * @param code				KeyCode de key de DB
	 * @param batch				Batch a usar
	 * @param viejos			Puestos viejos (los de la DB)
	 * @param nuevos			Puestos nuevos del usuario que reeemplazarn a los viejos
	 */
	void rlUpdatePuestos(uint32_t userID, KeyCode code, WriteBatch &batch,
				vector<Puesto> viejos, vector<Puesto> nuevos);

	/**
	 * Dado un vector de IDs y opcionalmente un nuevo valor de popularidad
	 * para algun uID, devuelve los uIDs ordenados por popularidad de mayor a menor
	 * @param uIDs				Vector de UIDs
	 * @param newUID			Null o UID con valor de popularidad no guardado en DB
	 * @param newPop			Null o Nuevo valor de popularidad
	 * @return
	 */
	vector<uint32_t> popSort(const vector <uint32_t> &uIDs,
			uint32_t *newUID = NULL, uint32_t *newPop = NULL);

	/**
	 * Inicializa la key para poder hacer busquedas por geolocacion
	 */
	void inicializarGeo();

 public:
	/**
	 * Constructor
	 * @param rutaArchivo				Ruta al archivo de base de datos, si no existe se crea
	 * @exception LevelDBException		Si falla algo al abrir la base de datos
	 */
	DBRaw(const string &rutaArchivo);

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
			const vector<char> &passHash);

	/**
	 * Agrega una nueva foto al usuario de la uID pedida
	 * Se espera que la foto este en formato jpg
	 * En caso de que ya existiera una foto para este usuario, la reemplaza
	 * @param uID						User ID del usuario
	 * @param foto						Foto
	 * @param data						Para el caso donde hay un batch y se cambiaron los datos
	 * @param batch						Null para escribir a db, puntero para batchear
	 * @param verifUID					Si debe realizarse la verificacion sobre el UID
	 * @exception NonexistentUserID		El uID no es válido
	 * @exception BadInputException		La foto no es un jpg
	 */
	void setFoto(uint32_t uID, const Foto &foto, const DatosUsuario *data = NULL,
			WriteBatch *batch = NULL, bool verifUID = true);

	/**
	 * Actualiza el resumen profesional del usuario
	 * @param uID						User ID
	 * @param resumen					El resumen personal arbitrario que escribio el usuario
	 * @param batch						Null para escribir a db, puntero para batchear
	 * @param verifUID					Si debe realizarse la verificacion sobre el UID
	 * @exception NonexistentUserID		El uID no es válido
	 */
	void setResumen(uint32_t uID, const string &resumen,
			WriteBatch *batch = NULL, bool verifUID = true);

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
			WriteBatch *batch = NULL, bool verifUID = true);

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

	/**
	 * Setea datos, foto, y resumen juntos
	 * @param uID						User ID
	 * @param datos						Datos
	 * @param skills					Vector de nombres de skills
	 * @param puestos					Vector de puestos
	 * @param resumen					Resumen, o null
	 * @param foto						Foto, o null
	 * @exception NonexistentUserID		El uID no es válido
	 * @exception BadInputException		La foto, si fue mandada, no es un jpg
	 * @exception NonexistentSkill		Alguno de los skills no existe en el shared
	 * @exception BadInputException		La foto no es un jpg
	 * @exception NonexistentPosition	Alguno de los puestos no existe en el shared
	 */
	void setPerfil(uint32_t uID, const DatosUsuario &datos,
			const vector<string> &skills, const vector<Puesto> &puestos,
			string *resumen = NULL, Foto *foto = NULL);

	/**
	 * Setea los skills de un usuario
	 * @param uID						User ID
	 * @param skills					Skills
	 * @param batch						Batch de ser necesario, null sino
	 * @param verifUID					Si se debe verificar que el uID sea valido
	 * @exception NonexistentUserID		El uID no es válido
	 * @exception NonexistentSkill		Alguno de los skills no existe en el shared
	 */
	void setSkills(uint32_t uID, vector<string> skills,
			WriteBatch *batch = NULL, bool verifUID = true);

	/**
	 * Devuelve los skills de un usuario
	 * @param uID						User ID
	 * @param verifUID					Si se debe verificar que sea un uid valido
	 * @exception NonexistentUserID		El uID no es válido
	 * @return							Vector de nombres de skill
	 */
	vector<string> getSkills(uint32_t uID, bool verifUID = true);

	/**
	 * @param uID						User ID
	 * @param puestos					Vecotr de puestos
	 * @param batch						Null para escribir a db, puntero para batchear
	 * @param verifUID					Si debe realizarse la verificacion sobre el UID
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception NonexistentPosition	Alguno de los puestos no existe en el shared
	 */
	void setPuestos(uint32_t uID, vector<Puesto> puestos,
			WriteBatch *batch = NULL, bool verifUID = true);

	/**
	 * Devuelve la lista de puestos de un usuario
	 * @param uID						User ID
	 * @param verifUID					Si se debe verificar que sea un uid valido
	 * @exception NonexistentUserID		El uID no es válido
	 * @return							Vector de puestos
	 */
	vector<Puesto> getPuestos(uint32_t uID, bool verifUID = true);

	/**
	 *  Hace login y devuelve un uid válido si la info es correcta
	 *  o lanza una excepcion si no lo es
	 * @param userName					Nombre de usuario
	 * @param passHash					Hash de la password del usuario de 256 bit
	 * @return							El user ID
	 * @exception NonexistentUsername	Datos de login incorrectos por username inexistente
	 * @exception BadPassword			Datos de login incorrectos por password incorrecto
	 */
	uint32_t login(const string &userName, const vector<char> &passHash);

	/**
	 * Realiza una búsqueda sobre los usuarios con los parametros dados.
	 * Si se pasa una categoria, posiblemente los vectores de skills y puestos
	 * sean modificados
	 * @param puestos					Lista de puestos. Null si no aplica.
	 * @param skill						Lista de skills. Null si no aplica.
	 * @param geolocacion				Punto geográfico. Null si no aplica.
	 * @param maxDist					Máxima distancia desde el punto. Infinito vale.
	 * @param sortPopularidad			Si se debe ordernar los resultados por popularidad.
	 * @return							Vector con todos los uIDs matcheantes
	 * @exception NonexistentSkill		Skill invalido
	 * @exception NonexistentPosition	Puesto invalido
	 * @exception BadInputException		Distancia máxima negativa
	 */
	vector<uint32_t> busquedaProfesional(const vector<string>
		*puestos, const vector<string> *skill, Geolocacion *geolocacion, float maxDist,
		bool sortPopularidad);

	/**
	 * Devuelve la popularidad del usuario
	 * @param uID						User ID
	 * @param verifUID					Si se debe verificar que el uid sea valido
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							#recomendaciones
	 */
	uint32_t getPopularidad(uint32_t uID, bool verifUID = true);

	/**
	 * Recomienda o desrecomienda un usuario a otro
	 * Si la recomendacion ya existia, se ignora
	 * @param uIDRecomendador			User ID de quien recomienda
	 * @param uIDRecomendado			User ID de quien es recomendad
	 * @param estado					True para recomendar, false para desrecomendar
	 * @exception NonexistentUserID		Algun uID es inválido
	 */
	void setRecomendacion(uint32_t uIDRecomendador,
			uint32_t uIDRecomendado, bool estado);

	/**
	 * Informa si un usuario ha recomendado a otro
	 * No verifica si existen los usuarios, en caso de que
	 * alguno no exista devuelve false
	 * @param uIDRecomendador			User ID de quien recomienda
	 * @param uIDRecomendado			User ID de quien es recomendad
	 * @return							True si el recomendado +1 por el recomendador
	 */
	bool esRecomendado(uint32_t uIDRecomendador, uint32_t uIDRecomendado);

	/**
	 * Devuelve los 10 usuarios más populares
	 * @return							Vector con todos los uIDs matcheantes
	 */
	vector<uint32_t> busquedaPopular(size_t count = DBConstNumBusquedaPop);

	/**
	 * Devuelve los 10 usuarios más populares, que posean un skill
	 * @param skill						Nombre del skill
	 * @return							Vector con todos los uIDs matcheantes
	 */
	vector<uint32_t> busquedaPopularSkill (const string &skill,
			size_t count = DBConstNumBusquedaPop);

	/**
	 * Devuelve los 10 usuarios más populares, que posean un puesto
	 * @param puesto					Nombre del puesto
	 * @return							Vector con todos los uIDs matcheantes
	 */
	vector<uint32_t> busquedaPopularPuesto (const string &puesto,
			size_t count = DBConstNumBusquedaPop);

	/**
	 * Envia un pedido de contacto
	 * @param uIDFuente					User ID de quien pide agregar contacto
	 * @param uIDDestino				User ID de quien será agregado
	 * @param mensaje					Mensaje definido por el usuario que envia la solicitud
	 * @exception NonexistentUserID		Alguno de los uIDs es inválido
	 * @exception AlreadyContacts		Ya eran contactos
	 */
	void solicitarContacto(uint32_t uIDFuente, uint32_t uIDDestino, const string &mensaje);

	/**
	 * Devuelve la lista de user IDs que quieren ser contactos del consultador
	 * @param uIDConsultador			User ID de quien busca la información
	 * @exception NonexistentUserID		El uID parametro es inválido
	 * @return							Vector de IDs
	 */
	vector<uint32_t> getSolicitudes(uint32_t uIDConsultador);

	/**
	 * Devuelve el mensaje de una solicitud particular
	 * @param uIDFuente					User ID de quien pide agregar contacto
	 * @param uIDDestino				User ID de quien será agregado
	 * @exception NonexistentRequest	No existe peticion entre estos usuarios
	 * @return							El mensaje
	 */
	string getMsgSolicitud(uint32_t uIDFuente, uint32_t uIDDestino);

	/**
	 * Devuelve el numero de peticiones que un usuario tiene pendientes
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID parametro es inválido
	 * @return							Numero de peticiones
	 */
	uint16_t getNumSolicitudes(uint32_t uID);

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
	 * @param batch						Null para escribir a db, puntero para batchear
	 * @exception NonexistentRequest	No existe peticion entre estos usuarios
	 */
	void eliminarSolicitud(uint32_t uIDFuente, uint32_t uIDDestino,
			WriteBatch *batch = NULL);

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
	vector<uint32_t> getContactos(uint32_t uID);

	/**
	 * Devuelve el numero de contactos del usuario
	 * @param uID						User ID
	 * @return							Número de contactos
	 * @exception NonexistentUserID		UserID inválido
	 */
	uint16_t getNumContactos(uint32_t uID);

	/**
	 * Devuelve si dos usuarios son contactos
	 * Devuelve false en caso que uno de los usuarios no exista tambien
	 * @param uID1				Usuario 1
	 * @param uID2				Usuario 2
	 * @return
	 */
	bool sonContactos(uint32_t uID1, uint32_t uID2);

	/**
	 * Devuelve el numero del ultimo mensaje en una conversacion
	 * El orden de los uIDs es irrelevante
	 * @param uID1			User ID1 de la conversación
	 * @param uID2			User ID2 de la conversación
	 * @return				Número del último mensaje
	 * @exception			UserIDs inválidos
	 */
	uint32_t getNumUltMensaje(uint32_t uID1, uint32_t uID2);

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
	vector<std::pair<uint32_t,string> > getMensajes(uint32_t uID1,
			uint32_t uID2, uint32_t numPrimMensaje, uint32_t numUltMensaje);

	/**
	 * Envia un mensaje
	 * @param uIDEmisor
	 * @param uIDReceptor
	 * @param mensaje
	 */
	void enviarMensaje(uint32_t uIDEmisor, uint32_t uIDReceptor, const string &mensaje);

	/**
	 * Devuelve el #mensaje del ultimo mensaje
	 * @param uIDLector
	 * @param uIDEmisor
	 * @return
	 */
	uint32_t getUltimoMsgNoLeido(uint32_t uIDLector, uint32_t uIDEmisor);

	/**
	 * Devuelve la lista de ids de los cuales el usuario recibio mensajes
	 * y aun no los leyo
	 * @param uID
	 * @return
	 */
	std::vector<uint32_t> getConversacionesNoLeidas(uint32_t uID);

	/**
	 * Marca una conversacion como leida
	 * @param uIDLector
	 * @param uIDEmisor
	 * @param ultMsgNumber				Nul para leer de db, puntero a val sino
	 * @param batch						Null para escribir a db, puntero para batchear
	 */
	void marcarConversacionLeida(uint32_t uIDLector, uint32_t uIDEmisor,
			uint32_t *ultMsgNumber = NULL, WriteBatch *batch = NULL);

};

#endif  /* APPSERVER_INCLUDE_DATABASE_DBRAW_H_ */
