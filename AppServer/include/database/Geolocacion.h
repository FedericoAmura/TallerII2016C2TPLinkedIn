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
	double longitud() const;
	/**
	 * Getter de la latitud
	 */
	double latitud() const;
	/**
	 * Devuelve el vector de bytes que representa una geolocacioón
	 */
	std::vector<char> toBytes() const;
	/**
	 * Igualdad a nivel de bits
	 */
	bool operator==(const Geolocacion &otro);
	/**
	 * Distancia euclidea
	 */
	double distancia(const Geolocacion &otro);
private:
	double _longitud;
	double _latitud;
};

#endif /* APPSERVER_INCLUDE_INCLUDE_DATABASE_GEOLOCACION_H_ */
