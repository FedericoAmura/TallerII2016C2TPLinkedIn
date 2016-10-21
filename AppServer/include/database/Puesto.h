#ifndef APPSERVER_INCLUDE_INCLUDE_DATABASE_PUESTO_H_
#define APPSERVER_INCLUDE_INCLUDE_DATABASE_PUESTO_H_

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
	Puesto(const std::string &puesto, Fecha fechaInicio, Fecha fechaFin);
	std::string puesto;
	Fecha fechaInicio;
	Fecha fechaFin;
};

#endif /* APPSERVER_INCLUDE_INCLUDE_DATABASE_PUESTO_H_ */
