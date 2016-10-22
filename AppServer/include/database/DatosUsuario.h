#ifndef APPSERVER_INCLUDE_DATABASE_DATOSUSUARIO_H_
#define APPSERVER_INCLUDE_DATABASE_DATOSUSUARIO_H_

#include <string>
#include "Fecha.h"
#include "Geolocacion.h"
#include "../../include/json11/json11.hpp"

using json11::Json;

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
	 * @param ciudad					Ciudad donde vive
	 * @param longitud					Longitud geográfica
	 * @param latitud					Latitud geográfica
	 */
	DatosUsuario(const std::string &nombre, const std::string &email, Fecha fechaNacimiento,
			const std::string &ciudad, Geolocacion geolocacion);
	/**
	 * Constructor por bytes de la base de datos
	 * @param slice						El slice levantado
	 */
	DatosUsuario(const char* byteArray);

	/**
	 * Devuelve los datos como cadena de bytes
	 * @return
	 */
	std::vector<uint8_t> toBytes();

private:
	std::string nombre;
	std::string email;
	std::string ciudad;
	Fecha fechaNacimiento;
	Geolocacion geolocacion;
};

#endif /* APPSERVER_INCLUDE_DATABASE_DATOSUSUARIO_H_ */
