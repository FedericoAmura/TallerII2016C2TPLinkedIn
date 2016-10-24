#include "../../include/database/Geolocacion.h"

Geolocacion::Geolocacion(double longitud, double latitud) {
	_longitud = longitud;
	_latitud = latitud;
}

Geolocacion::Geolocacion(const Geolocacion& geolocacion)
	: _latitud(geolocacion._latitud), _longitud(geolocacion._longitud) {}

Geolocacion::Geolocacion(const char *byteArray) {
	assign(byteArray);
}

void Geolocacion::assign(const char *byteArray) {
	std::copy(byteArray, byteArray+sizeof(_longitud), (char*)&_longitud);
	std::copy(byteArray+sizeof(_longitud),
			byteArray+sizeof(_longitud)+sizeof(_latitud), (char*)&_latitud);
}

Geolocacion::Geolocacion() {}

double Geolocacion::longitud() const {
	return _longitud;
}

double Geolocacion::latitud() const {
	return _latitud;
}

#include <cstring>
std::vector<char> Geolocacion::toBytes() const {
	std::vector<char> result(sizeof(double)*2);
	std::vector<char>::iterator it = std::copy((char*)&_longitud, (char*)(&_longitud+1), result.begin());
	std::copy((char*)&_latitud, (char*)(&_latitud+1), it);
	return result;
}
