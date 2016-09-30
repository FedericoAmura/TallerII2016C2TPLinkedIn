#ifndef APPSERVER_INCLUDE_DATABASE_DBRAW_H_
#define APPSERVER_INCLUDE_DATABASE_DBRAW_H_

#include "../leveldb/db.h"

/**
 * Clase wrapper de levelDB con el modelo de datos propio de la aplicación
 * Maneja tipos de datos std o propios y devuelve tipos de datos std o propios
 */
class DBRaw {
 private:

 public:
	/**
	 * Constructor
	 * @param rutaArchivo 		Ruta al archivo, si no existe se crea
	 */
	DBRaw(const std::string &rutaArchivo);
	virtual ~DBRaw();
	/**
	 * Registrar un nuevo usuario
	 * @exception		Si hay errores en los datos provistos
	 * @return			El uID (user ID) del nuevo usuario creado
	 */
	uint32_t registrarse();
	/**
	 * Agrega una nueva foto al usuario de la uID pedida
	 * Se espera que la foto este en formato jpg
	 * En caso de que ya existiera una foto para este usuario, la reemplaza
	 * @param uID		User ID del usuario
	 * @param foto		Array de bytes de la foto
	 * @exception		Si el uID no es válido
	 * @exception		Si la foto no es un jpg
	 */
	void agregarFoto(uint32_t uID, void* foto);
	/**
	 * Agrega un skill a un usuario
	 * @param uID		User ID
	 * @param skill		Nombre del skill
	 * @exception		El uID es inválido
	 * @exception		El skill es inválido (no existe en el shared server)
	 */
	void agregarSkill(uint32_t uID, const std::string &skill);
	/**
	 * Elimina un skill del usuario
	 * @param uID		User ID
	 * @param skill		Nombre del Skill
	 * @exception		El uID es inválido
	 * @exception		El skill es inválido (no existe en el usuario)
	 */
	void eliminarSkill(uint32_t uID, const std::string &skill);
	/**
	 * Agrega un puesto de trabajo a los datos de un usuario
	 * Las fechas deberan venir en el siguiente formato:
	 * 32 bytes = 16 bytes año, 8 bytes mes, 8 bytes día
	 * En binario: AAAAAAAA AAAAAAAA MMMMMMMM DDDDDDDD
	 * Para posicion actual se tomara como fecha de fin = 2^32 - 1
	 * Es decir año 65535 mes 256 dia 256
	 * Se chequeara que la fecha sea valida siempre
	 * @param uID			User ID
	 * @param puesto		Nombre del puesto
	 * @param fechaInicio	Fecha de inicio
	 * @param fechaFin		Fecha de fin
	 * @exception			El uID es inválido
	 * @exception			El puesto no es válido (no existe en el shared server)
	 * @exception			Las fechas son de alguna forma invalidas
	 */
	void agregarPuestoLaboral(uint32_t uID, const std::string &puesto,
			uint32_t fechaInicio, uint32_t fechaFin);
	/**
	 * Elimina la posición pedida al usuario del dado uID
	 * Se piden las fechas porque el usuario puede haber tenido el mismo
	 * trabajo más de una vez, y no hay forma de identificarlo sino
	 * @param uID			User ID
	 * @param puesto		Nombre del puesto
	 * @param fechaInicio	Fecha de inicio
	 * @param fechaFin		Fecha de fin
	 * @exception			El uID es inválido
	 * @exception			El puesto no es válido (no existe en el usuario)
	 * @exception			Las fechas no concuerdan con las del usuario
	 */
	void eliminarPosicion(uint32_t uID, const std::string &puesto,
			uint32_t fechaInicio, uint32_t fechaFin);
	/**
	 * Actualiza el mail del usuario del uID dado
	 * No se chequea que el mail en cuestion realmente exista
	 * TODO: Hallar la manera de verificar si un mail existe
	 * @param uID			User ID
	 * @param email			El email
	 * @exception			El uID es inválido
	 */
	void actualizarEMail(uint32_t uID, const std::string &email);
	/**
	 * Actualiza la ciudad del usuario del uID dado
	 * No se chequea que la ciudad en cuestión realmente exista
	 * @param uID			User ID
	 * @param ciudad		Nombre de la ciudad
	 * @exception			El uID es inválido
	 */
	void actualizarCiudad(uint32_t uID, const std::string &ciudad);
	/**
	 * Actualiza la locación geografica del usuario a la dada
	 * @param uID			User ID
	 * @param latitud		Latitud, float segun estandar Android
	 * @param longitud		Longitud, float segun estandar Android
	 */
	void actualizarGeolocacion(uint32_t uID,float latitud, float longitud);

	/**
	 * Actualiza el resumen profesional del usuario
	 * @param uID			User ID
	 * @param resumen		El resumen personal arbitrario que escribio el usuario
	 * @exception			El uID es inválido
	 */
	void actualizarresumen(uint32_t uID, const std::string &resumen);
	/**
	 * Devuelve todos los datos de usuario que el sistema tiene,
	 * salvo por la foto
	 * @param uID			User ID
	 * @return				TODO: crear clase
	 */
	int getDatos(uint32_t uID);
	/**
	 * Devuelve la foto del usuario
	 * En caso de no existir devuelve una por defecto
	 * @param uID			User ID
	 */
	void* getFoto(uint32_t uID);
	/**
	 * Devuelve el thumbnail de 128x128 de la foto del usuario
	 * En caso de no existir devuelve uno por defecto
	 * @param uID			User ID
	 */
	void* getFotoThumbnail(uint32_t uID);
	/**
	 *  Hace login y devuelve un uid válido si la info es correcta
	 *  o lanza una excepcion si no lo es
	 * @param username		Nombre de usuario
	 * @param passwordHash	Hash de la password del usuario de 256 bit
	 * @return				El user ID
	 * @exception			Datos de login incorrectos por username inexistente
	 * @exception			Datos de login incorrectos por password incorrecto
	 */
	uint32_t login(const std::string &username, const void* passwordHash);
	/**
	 * Realiza una búsqueda sobre los usuarios con los parametros dados.
	 * Si se pasa una categoria, posiblemente los vectores de skills y puestos
	 * sean modificados
	 * @param puestos			Lista de puestos. Null si no aplica.
	 * @param skill				Lista de skills. Null si no aplica.
	 * @param categorias		Lista de categorias. Null si no aplica.
	 * @param geolocacion		Punto geográfico. Null si no aplica.
	 * @param maxDist			Máxima distancia desde el punto. Infinito vale.
	 * @param sortPopularidad	Si se debe ordernar los resultados por popularidad.
	 * @return					Vector con todos los uIDs matcheantes
	 * @exception				Skill/puesto/categoria invalidos
	 * @exception				Distancia máxima negativa
	 */
	std::vector<uint32_t> busqueda_profresional(std::vector<std::string>
		&puestos, std::vector<std::string> &skill, const std::vector<std::string>
		&categorias, std::pair<float,float> geolocacion, float maxDist,
		bool sortPopularidad);
	/**
	 * Realiza una busqueda sobre solo los usuarios más populares
	 * @param conteo			Cuantos de los más populares se usaran para la busqueda
	 * @param puestos			Lista de puestos. Null si no aplica.
	 * @param skill				Lista de skills. Null si no aplica.
	 * @param categorias		Lista de categorias. Null si no aplica.
	 * @exception				Skill/puesto/categoria invalidos
	 * @return					Vector con todos los uIDs matcheantes
	 */
	std::vector<uint32_t> busqueda_popularidad (uint conteo,
			std::vector<std::string> &puestos, std::vector<std::string>
			&skill, const std::vector<std::string> &categorias);
	/**
	 * Envia un pedido de contacto
	 * @param uIDFuente			User ID de quien pide agregar contacto
	 * @param uIDDestino		User ID de quien será agregado
	 * @param mensaje			Mensaje definido por el usuario que envia la solicitud
	 * @exception				Alguno de los uIDs es inválido
	 */
	void solicitarContacto(uint32_t uIDFuente, uint32_t uIDDestino, const std::string &mensaje);
	/**
	 * Elimina la solicitud de contacto y vuelve a ambos contactos uno del otro
	 * @param uIDFuente			User ID de quien pide agregar contacto
	 * @param uIDDestino		User ID de quien será agregado
	 * @exception				Alguno de los uIDs es inválido
	 */
	void aceptarContacto(uint32_t uIDFuente, uint32_t uIDDestino);
	/**
	 * Elimina la solicitud de contacto
	 * @param uIDFuente			User ID de quien pide agregar contacto
	 * @param uIDDestino		User ID de quien será agregado
	 * @exception				Alguno de los uIDs es inválido
	 */
	void declinarContactor(uint32_t uIDFuente, uint32_t uIDDestino);
	/**
	 * Devuelve la lista de contactos del usuario
	 * @param uID			User ID
	 * @return				Vector de user IDs
	 * @exception			UserID inválido
	 */
	std::vector<uint32_t> getContactos(uint32_t uID);
	/**
	 * Devuelve el numero de contactos del usuario
	 * @param uID			User ID
	 * @return				Número de contactos
	 * @exception			UserID inválido
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
	std::vector<std::pair(uint32_t,std::string)> getMensajes(uint32_t uID1,
			uint32_t uID2, uint32_t numUltMensaje, uint32_t numPrimMensaje);

};

#endif  /* APPSERVER_INCLUDE_DATABASE_DBRAW_H_ */
