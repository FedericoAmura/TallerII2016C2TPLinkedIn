#ifndef APPSERVER_INCLUDE_INCLUDE_DATABASE_GEOLOCACION_H_
#define APPSERVER_INCLUDE_INCLUDE_DATABASE_GEOLOCACION_H_

#include <vector>
#include <stdint.h>

/**
 * Clase que representa un par de coordenadas geograficas
 */
class Geolocacion
{
public:
	/**
	 * Constructor explicito
	 * @param longitud
	 * @param latitud
	 * @exception BadGeolocation		Numeros fuera de rango
	 */
	Geolocacion(double longitud, double latitud);
	/**
	 * Constructor por copia
	 * @param geolocacion
	 */
	Geolocacion(const Geolocacion &geolocacion);
	/**
	 * Constructor por byte array
	 */
	Geolocacion(const char *byteArray);
	/**
	 * Asignacion por byte array
	 */
	void assign(const char *byteArray);
	/**
	 * Constructor por defecto no inicializado
	 */
	Geolocacion();
	/**
	 * Getter de la longitud
	 */
	double longitud();
	/**
	 * Getter de la latitud
	 */
	double latitud();
	/**
	 * Devuelve el vector de bytes que representa una geolocacioón
	 */
	std::vector<uint8_t> toBytes();
private:
	double _longitud;
	double _latitud;
};

#endif /* APPSERVER_INCLUDE_INCLUDE_DATABASE_GEOLOCACION_H_ */
