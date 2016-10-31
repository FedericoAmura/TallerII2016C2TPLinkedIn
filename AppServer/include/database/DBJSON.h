#ifndef APPSERVER_INCLUDE_DATABASE_DBJSON_H_
#define APPSERVER_INCLUDE_DATABASE_DBJSON_H_

#include "DBRaw.h"
#include "../include/json11/json11.hpp"
#include "../common/Utils.h"
#include <map>

using json11::Json;
using std::string;
using std::vector;

/**
 * Wrapper de la base de datos que toma jsons y/o parametros y devuelve jsons
 * No hace autenticaciones, asi que en caso de comandos que requieran autenticacion
 * Se debe hacer previamente (Y asi mismo para aquellos donde haya que corroborar
 * que el sender sea el mismo que el que viene en el json)
 */
class DBJSON {
 private:
	DBRaw* db;
	SharedServerHandler* sharedServerHandler;
    /* temporal */
    std::map<string,string> tokens;

 public:
	DBJSON(SharedServerHandler* sharedServerHandler, DBRaw *db);
	virtual ~DBJSON();

    /**
     * Generar token
     * @param json                      Información de usuario
     * @return                          Un token de sesión
     */
    string generarToken(const Json &json);

    /**
     * Validar Token
     * @param json                      Información de usuario
     * @exception NonexistentToken      El token no existe
     * @exception TokenHasExpired       El token ha expirado
     * @return                          true si el token el válido
     */
    bool validar_token(const string &token);

	/**
	 * Registrar un nuevo usuario
	 * @param json						Ver documentacion API: "Registro de un nuevo usuario"
	 * @exception PreexistentUsername	Nombre de usuario ya tomado
	 * @exception NonexistentEmail		El email no es un mail valido
	 * @exception BadGeolocation		Latitud/longitud fuera de rango
	 * @exception MalformedDate			Fecha malformada
	 * @exception BadPasswordSize		La password provista no es un hash de 32 bytes
	 * @exception BadInputException		Falta algun campo del JSON
	 * @return							El uID (user ID) del nuevo usuario creado
	 */
	uint32_t registrarse(const Json &json);

	/**
	 * Hace login y devuelve el user ID en caso de ser un login exitoso
	 * @param json						{"username" : <username>, "password" : <stringbase64password>}
	 * @exception NonexistentUsername	Datos de login incorrectos por username inexistente
	 * @exception BadPassword			Datos de login incorrectos por password incorrecto
	 * @exception BadPasswordSize		La password provista no es un hash de 32 bytes
	 * @exception BadInputException		Falta algun campo del JSON
	 * @return							Ver documentacion API: "Login"
	 */
	uint32_t login(const Json &json);

	/**
	 * Devuelve los datos de perfil, con foto y resumen
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API: "Perfil (...) : Consulta"
	 */
	Json getDatos(uint32_t userID);

	/**
	 * Devuelve nombre, popularidad, ciudad, thumbnail
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API: "Datos breves (...)"
	 */
	Json getDatosBrief(uint32_t userID);


	/**
	 * Guarda nuevos datos de perfil, foto y resumen optativos
	 * @param uID						User ID
	 * @param json						Ver documentacion API: "Perfil (...) : Actualizar"
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception NonexistentSkill		Alguno de los skills es invalido (No existe en el shared)
	 * @exception NonexistentPosition	Alguno de los puestos es invalido (No existe en el shared)
	 */
	void setDatos(uint32_t userID, const Json &json);

	/**
	 * Devuelve el resumen del usuario
	 * @param userID					User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return 							Ver documentacion API: "Resumen: Consultar"
	 */
	Json getResumen(uint32_t userID);

	/**
	 * Actualiza el resumen por uno nuevo
	 * @param userID					User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception BadInputException		Falta algun campo del JSON
	 * @param json						Ver documentacion API: "Resumen: Actualizar"
	 */
	void setResumen(uint32_t userID, const Json &json);

	/**
	 * Devuelve la foto del usuario
	 * @param userID					User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API: "Foto: Consulta"
	 */
	Json getFoto(uint32_t userID);

	/**
	 * Devuelve el thumbnail (64x64) de la foto del usuario
	 * @param userID					User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API: "Foto: Consulta"
	 */
	Json getFotoThumbnail(uint32_t userID);

	/**
	 * Carga una nueva foto de usuario
	 * @param userID					User ID
	 * @param json						Ver documentacion API: "Foto: Actualizar"
	 * @exception NonexistentUserID		El uID es inválido
	 * @exception TooBigException		Foto de más de 4 MBs
	 * @exception BadInputException		Falta algun campo del JSON
	 */
	void setFoto(uint32_t userID, const Json &json);

	/**
	 * Realiza una búsqueda sobre los usuarios con los parametros dados.
	 * Si se pasa una categoria, posiblemente los vectores de skills y puestos
	 * sean modificados
	 * @param puestos					Lista de puestos. Null si no aplica.
	 * @param skill						Lista de skills. Null si no aplica.
	 * @param categorias				Lista de categorias. Null si no aplica.
	 * @param geolocacion				Punto geográfico. Null si no aplica.
	 * @param maxDist					Máxima distancia desde el punto. Infinito si no aplica.
	 * @param sortPopularidad			Si se debe ordernar los resultados por popularidad.
	 * @return							Ver documentación API: "Búsquedas sobre usuarios"
	 * @exception NonexistentSkill		Algun skill es invalido
	 * @exception NonexistentPosition	Algun puesto es invalido
	 * @exception NonexistentCategory	Alguna categoria es invalida
	 * @exception BadInputException		Distancia máxima negativa
	 */
	Json busqueda_profresional(const vector<string>
		*puestos, const vector<string> *skill, const vector<string>
		*categorias, Geolocacion *geolocacion, float maxDist,
		bool sortPopularidad);

	/**
	 * Devuelve los user ids de quienes enviaron peticiones para el usuario dado
	 * @param userID					User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API: "Consultar peticiones pendientes"
	 */
	Json getPeticionesPendientes(uint32_t userID);

	/**
	 * Devuelve el numero de peticiones pendientes de un usuario
	 * @param userID					User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API: "Consultar número de peticiones pendientes"
	 */
	Json getNumPeticionesPendientes(uint32_t userID);

	/**
	 * Devuelve una peticion dados dos IDs de usuario
	 * @param userID1					Un user ID
	 * @param userID2					El otro user ID
	 * @exception NonexistentRequest	No existe peticion entre estos usuarios
	 * @return							Ver documentacion API: "Consultar número de peticiones pendientes
	 */
	Json getPeticion(uint32_t userID1, uint32_t userID2);

	/**
	 * Acepta una petición entre dos usuarios
	 * @param userID1					Un user ID
	 * @param userID2					El otro user ID
	 * @exception NonexistentRequest	No existe peticion entre estos usuarios
	 */
	void aceptarPeticion(uint32_t userID1, uint32_t userID2);

	/**
	 * Rechaza una petición entre dos usuarios
	 * @param userID1					Un user ID
	 * @param userID2					El otro user ID
	 * @exception NonexistentRequest	No existe peticion entre estos usuarios
	 */
	void declinarPeticion(uint32_t userID1, uint32_t userID2);

	/**
	 * Devuelve la lista de contactos para un usuario
	 * @param userID					El user ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API: "Consultar contactos"
	 */
	Json getContactos(uint32_t userID);

	/**
	 * Crea una peticion de contacto (sobreescribe si ya existe)
	 * @param json						Ver documentacion API: "Crear petición (...)"
	 * @exception NonexistentUserID		Algun user ID no es valido
	 * @exception BadInputException		Falta algun campo del JSON
	 * @exception AlreadyContacts		Ya eran contactos
	 */
	void crearPeticion(const Json &json);

	/**
	 * Devuelve si dos usuarios son contactos o no
	 * @param userID1					Un user ID
	 * @param userID2					El otro user ID
	 * @return							Si es contacto o no
	 */
	bool esContacto(uint32_t userID1, uint32_t userID2);

	/**
	 * Elimina contacto entre dos usuarios
	 * @param userID1					Un user ID
	 * @param userID2					El otro user ID
	 * @exception NonexistentContact	No eran contactos
	 */
	void eliminarContacto(uint32_t userID1, uint32_t userID2);

	/**
	 * Devuelve si un usuario recomendo a otro
	 * @param userIDRecomendador		Quien recomienda
	 * @param userIDRecomendado			Quien recibe la recomendación
	 * @exception NonexistentUserID		Alguno de los uID es inválido
	 * @return							Ver documentación API: "Ver si un usuario particular recomnedó(...)"
	 */
	Json esRecomendado(uint32_t userIDRecomendador, uint32_t userIDRecomendado);

	/**
	 * Recomienda o desrecomienda usuarios
	 * @param json						Ver documentación API: "Recomendar a un usuario"
     * @exception NonexistentUserID		Alguno de los uID es inválido
     * @exception BadInputException		Falta algun campo del JSON
	 */
	void actualizarRecomendacion(const Json &json);

	/**
	 * Devuelve los 10 usuarios más populares
	 * @return					Ver documentación API: "Consultar 10 más populares"
	 */
	Json getPopulares();

	/**
	 * Devuelve los 10 usuarios más populares que tengan el skill dado
	 * @param skill				El skill filtro
	 * @return					Ver documentación API: "Consultar 10 más populares por skill"
	 */
	Json getPopularesPorSkill(const string &skill);

	/**
	 * Devuelve los 10 usuarios más populares que tengan el skill dado
	 * @param position			La position filtro
	 * @return					Ver documentación API: "Consultar 10 más populares por posición"
	 */
	Json getPopularesPorPosition(const string &position);

	/**
	 * Devuelve la cantidad de mensajes nuevos en chats
	 * @param userID					User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentación API: "Consultar cantidad de mensajes nuevos"
	 */
	Json getChatNuevos(uint32_t userID);

	/**
	 * Marca un chat como leido
	 * @param json						Ver documentación API: "Marcar mensajes como leidos"
	 * @exception NonexistentChat		No existe el chat especificado
	 * @exception BadInputException		Falta algun campo del JSON
	 */
	void marcarChatLeido(const Json &json);

	/**
	 * Envia un mensaje entre usuarios
	 * @param json						Ver documentación API: "Enviar mensaje"
	 * @exception NonexistentUserID		El uID destinatario es inválido
	 * @exception BadInputException		Falta algun campo del JSON
	 */
	void enviarMensaje(const Json &json);

	/**
	 * Devuelve el numero del ultimo mensaje en un chat entre dos usuarios
	 * @param userID1					Un user ID
	 * @param userID2					El otro user ID
	 * @exception NonexistentUserID		Algun user ID es inválido
	 * @return							Ver documentación API: "Consultar número de último mensaje"
	 */
	Json getNumLastMensaje(uint32_t userID1, uint32_t userID2);

	/**
	 * Devuelve los mensajes comprendidos entre el primero y ultimo para un chat entre
	 * dos usuarios
	 * @param userID1					Un user ID
	 * @param userID2					El otro user ID
	 * @param primerMensaje				Número de primer mensaje
	 * @param ultMensaje				Número de ultimo mensaje
	 * @exception NonexistentUserID		Algun user ID es inválido
	 * @return							Ver documentación API: "Consultar mensajes"
	 */
	Json getMensajes(uint32_t userID1, uint32_t userID2,
			uint32_t primerMensaje, uint32_t ultMensaje);

	/**
	 * Devuelve la lista de todas las categorias
	 * @return							Ver documentación API: "Categorías"
	 */
	Json getCategorias();

	/**
	 * Devuelve las positions que pertenecen a una categoria
	 * @param categoria					Nombre de la categoria
	 * @exception NonexistentCategory	La categoria no existe
	 * @return							Ver documentación API: "Consultar position por categoría"
	 */
	Json getPositionsPorCategoria(const string &categoria);

	/**
	 * Devuelve los skills que pertenecen a una categoria
	 * @param categoria					Nombre de la categoria
	 * @exception NonexistentCategory	La categoria no existe
	 * @return							Ver documentación API: "Consultar skill por categoría"
	 */
	Json getSkillsPorCategoria(const string &categoria);

	/**
	 * Devuelve informacion sobre una position particular
	 * @param position					Nombre de la position
	 * @exception NonexistentPosition	La position no existe
	 * @return							Ver documentación API: "Consultar position particular"
	 */
	Json getPosition(const string &position);

	/**
	 * Devuelve informacion sobre un skill particular
	 * @param skill						Nombre del skill
	 * @exception NonexistentSkill		El skill no existe
	 * @return							Ver documentación API: "Consultar skill particular"
	 */
	Json getSkill(const string &skill);
};

#endif  /* APPSERVER_INCLUDE_DATABASE_DBJSON_H_ */
