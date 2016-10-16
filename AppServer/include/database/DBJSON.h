#ifndef APPSERVER_INCLUDE_DATABASE_DBJSON_H_
#define APPSERVER_INCLUDE_DATABASE_DBJSON_H_

#include "../include/json11/json11.hpp"

using json11::Json;
using std::string;

class DBJSON {
 private:
	DBRaw* db;
	SharedServerHandler* sharedServerHandler;
 public:
	DBJSON();
	virtual ~DBJSON();

	/**
	 * Registrar un nuevo usuario
	 * @exception PreexistentUsername	Nombre de usuario ya tomado
	 * @exception NonexistentEmail		El email no es un mail valido
	 * @return							El uID (user ID) del nuevo usuario creado
	 */
	uint32_t registrarse(const Json &json);

	/**
	 * Hace login y devuelve el user ID en caso de ser un login exitoso
	 * @param json						Ver documentacion API
	 * @return							Ver documentacion API
	 * @exception NonexistentUsername	Datos de login incorrectos por username inexistente
	 * @exception BadPassword			Datos de login incorrectos por password incorrecto
	 */
	uint32_t login(const Json &json);

	/**
	 * Devuelve los datos de perfil, salvo foto y resumen
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API
	 */
	Json getDatos(uint32_t userID);

	/**
	 * Devuelve nombre, popularidad, ciudad, thumbnail
	 * @param uID						User ID
	 * @exception NonexistentUserID		El uID es inválido
	 * @return							Ver documentacion API
	 */
	Json getDatosBrief(uint32_t userID);


	/**
	 * Devuelve los datos de perfil, salvo foto y resumen
	 * @param uID						User ID
	 * @param json						Ver documentacion API
	 * @exception NonexistentUserID		El uID es inválido
	 */
	void actualizarDatos(uint32_t userID, const Json &json);

	Json getResumen(uint32_t userID);

	void actualizarResumen(uint32_t userID, const Json &json);

	Json getFoto(uint32_t userID);

	Json getFotoThumbnail(uint32_t userID);

	void actualizarFoto(uint32_t userID, const Json &json);

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
	Json busqueda_profresional(const std::vector<string>
		*puestos, const std::vector<string> *skill, const std::vector<string>
		*categorias, Geolocacion *geolocacion, float maxDist,
		bool sortPopularidad);

	Json getPeticionesPendientes(uint32_t userID);

	Json getNumPeticionesPendientes(uint32_t userID);

	Json getPeticion(uint32_t userID1, uint32_t userID2);

	void aceptarPeticion(uint32_t userID1, uint32_t userID2);

	void declinarPeticion(uint32_t userID1, uint32_t userID2);

	Json getContactos(uint32_t userID);

	/**
	 * Debe verificarse en un nivel superior si concuerda el usuario con el
	 * creador de la peticion segun el JSON
	 * @param json
	 */
	void crearPeticion(const Json &json);

	bool esContacto(uint32_t userID1, uint32_t userID2);

	void eliminarContacto(uint32_t userID1, uint32_t userID2);

	Json esRecomendado(uint32_t userIDRecomendador, uint32_t userIDRecomendado);

	/**
	 * Recomienda o desrecomienda usuarios
	 * Debe verificarse en un nivel superior si concuerda el usuario con el
	 * creador de la peticion segun el JSON
	 * @param json
	 */
	void actualizarRecomendacion(const Json &json);

	Json getPopulares();

	Json getPopularesPorSkill(const string &skill);

	Json getPopularesPorPosition(const string &position);

	Json getChatNuevos(uint32_t userID);

	void marcarChatLeido(const Json &json);

	/**
	 * Debe verificarse en un nivel superior si concuerda el usuario con el
	 * creador del mensaje segun el JSON
	 * @param json
	 */
	void enviarMensaje(const Json &json);

	Json getNumLastMensaje(uint32_t userID1, uint32_t userID2);

	Json getMensajes(uint32_t userID1, uint32_t userID2,
			uint32_t primerMensaje, uint32_t ultMensaje);

	Json getCategorias();

	Json getPositionsPorCategoria(const string &categoria);

	Json getSkillsPorCategoria(const string &categoria);

	Json getPosition(const string &position);

	Json getSkill(const string &skill);


};

#endif  /* APPSERVER_INCLUDE_DATABASE_DBJSON_H_ */
