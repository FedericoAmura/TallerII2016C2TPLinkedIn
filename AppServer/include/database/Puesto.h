#ifndef APPSERVER_INCLUDE_INCLUDE_DATABASE_PUESTO_H_
#define APPSERVER_INCLUDE_INCLUDE_DATABASE_PUESTO_H_

#include <string>
#include <vector>
#include "DBExceptions.h"
#include "Fecha.h"

/**
 * Clase que encapsula un puesto de trabajo y un periodo de tiempo
 */
class Puesto
{
 public:
	/**
	 * Constructor a partir de datos explicitos
	 * @param puesto						Nombre del puesto
	 * @param fechaInicio					Fecha de inicio
	 * @param fechaFin						Fecha de fin
	 * @exception TimeTravel				Fin < Inicio
	 */
	Puesto(const std::string &puesto, Fecha fechaInicio, Fecha fechaFin);
	/**
	 * Constructor a partir de byte array
	 * @param datosBinarios
	 */
	Puesto(const char* datosBinarios);
	/**
	 * Devuelve la tira de bytes que representa el puesto
	 */
	std::vector<char> toBytes() const;
	/**
	 * Devuelve el largo en bytes
	 */
	size_t length() const;
	std::string puesto;
	Fecha fechaInicio;
	Fecha fechaFin;
};

#endif /* APPSERVER_INCLUDE_INCLUDE_DATABASE_PUESTO_H_ */
