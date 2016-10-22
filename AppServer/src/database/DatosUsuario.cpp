#include "../../include/database/DatosUsuario.h"

DatosUsuario::DatosUsuario(const std::string& nombre, const std::string& email,
		Fecha fechaNacimiento, const std::string& ciudad,
		Geolocacion geolocacion) : nombre(nombre), email(email), ciudad(ciudad),
		fechaNacimiento(fechaNacimiento), geolocacion(geolocacion) {}

DatosUsuario::DatosUsuario(const char *byteArray) {
	char offset = 0;
	size_t len = byteArray[offset++];
	nombre.assign(byteArray+offset, len);
	offset += len;
	len = byteArray[offset++];
	email.assign(byteArray+offset, len);
	offset += len;
	len = byteArray[offset++];
	ciudad.assign(byteArray+offset, len);
	offset += len;
	fechaNacimiento.assign(byteArray+offset);
	offset += 4;
	geolocacion.assign(byteArray+offset);
}

std::vector<uint8_t> DatosUsuario::toBytes() {
}
