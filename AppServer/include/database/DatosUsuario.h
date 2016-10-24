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
 public:
	/**
	 * Constructor por parametro (pone el foto ID por defecto en 0)
	 * @param nombre					Nombre y apellido de la persona
	 * @param email						Dirección de email
	 * @param ciudad					Ciudad donde vive
	 * @param fechaNacimiento			Fecha de nacimiento segun el formato establecido
	 * @param longitud					Longitud geográfica
	 * @param latitud					Latitud geográfica
	 * @param fotoID					ID de foto, generalmente no esta cargado y es 0 por defecto
	 */
	DatosUsuario(const std::string &nombre, const std::string &email, const std::string &ciudad,
			const Fecha &fechaNacimiento, Geolocacion geolocacion, uint32_t fotoID = 0);
	/**
	 * Constructor por bytes de la base de datos
	 * @param slice						El slice levantado
	 */
	DatosUsuario(const char* byteArray);

	/**
	 * Devuelve los datos como cadena de bytes
	 * @return
	 */
	std::vector<char> toBytes() const;

//Atributos
	std::string nombre;
	std::string email;
	std::string ciudad;
	Fecha fechaNacimiento;
	Geolocacion geolocacion;
	uint32_t fotoID;
};

#endif /* APPSERVER_INCLUDE_DATABASE_DATOSUSUARIO_H_ */
