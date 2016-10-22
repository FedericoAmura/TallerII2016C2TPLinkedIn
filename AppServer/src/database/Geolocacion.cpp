#include "../../include/database/Geolocacion.h"
#include <cstring>

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
	std::memcpy(&_longitud, byteArray, sizeof(double));
	std::memcpy(&_latitud, byteArray+sizeof(double), sizeof(double));
}

Geolocacion::Geolocacion() {}

double Geolocacion::longitud() {
	return _longitud;
}

double Geolocacion::latitud() {
	return _latitud;
}

std::vector<uint8_t> Geolocacion::toBytes() {
	std::vector<uint8_t> result(sizeof(double)*2);
	std::memcpy(result.data(), &_longitud, sizeof(double));
	std::memcpy(result.data()+sizeof(double), &_latitud, sizeof(double));
	return result;
}
