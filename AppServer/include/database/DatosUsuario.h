#ifndef APPSERVER_INCLUDE_DATABASE_DATOSUSUARIO_H_
#define APPSERVER_INCLUDE_DATABASE_DATOSUSUARIO_H_

#include "Fecha.h"
#include "Geolocacion.h"

/**
 * Clase que representa varios de los datos de usuario agrupados
 */
class DatosUsuario
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
	DatosUsuario(const std::string &nombre, const std::string &email, Fecha fechaNacimiento,
			const std::string &userName, const uint8_t *passHash, const std::string &ciudad,
			Geolocacion geolocacion);
	std::string nombre;
	std::string email;
	Fecha fechaNacimiento;
	std::string userName;
	uint8_t passHash;
	std::string ciudad;
	Geolocacion geolocacion;
};

#endif /* APPSERVER_INCLUDE_DATABASE_DATOSUSUARIO_H_ */
