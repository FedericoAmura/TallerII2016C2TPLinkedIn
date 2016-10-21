#ifndef APPSERVER_INCLUDE_INCLUDE_DATABASE_GEOLOCACION_H_
#define APPSERVER_INCLUDE_INCLUDE_DATABASE_GEOLOCACION_H_

class Geolocacion
{
public:
	/**
	 * Constructor
	 * @param longitud
	 * @param latitud
	 * @exception BadGeolocation		Numeros fuera de rango
	 */
	Geolocacion(double longitud, double latitud);
	double longitud();
	double latitud();
private:
	double _longitud;
	double _latitud;
};

#endif /* APPSERVER_INCLUDE_INCLUDE_DATABASE_GEOLOCACION_H_ */
