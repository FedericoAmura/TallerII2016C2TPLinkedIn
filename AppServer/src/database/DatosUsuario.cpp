#include "../../include/database/DatosUsuario.h"

DatosUsuario::DatosUsuario(const std::string& nombre, const std::string& email,
		const std::string& ciudad, const Fecha &fechaNacimiento,
		Geolocacion geolocacion, uint32_t fotoID) : nombre(nombre), email(email), ciudad(ciudad),
		fechaNacimiento(fechaNacimiento), geolocacion(geolocacion), fotoID(fotoID) {
	//TODO: Validar tamaños maximos
}

#include <iostream>
DatosUsuario::DatosUsuario(const char *byteArray) {
	int offset = 0;

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
	offset += 16;
	std::copy(byteArray+offset, byteArray+offset+4,(char*) &fotoID);
}

std::vector<char> DatosUsuario::toBytes() const {
	std::vector<char> result(1+nombre.length()+1+email.length()+1+ciudad.length()+4+16+4);
	std::vector<char>::iterator it = result.begin();
	*it = (uint8_t) nombre.length();
	++it;
	it = std::copy(nombre.begin(), nombre.end(), it);
	*it = (uint8_t) email.length();
	++it;
	it = std::copy(email.begin(), email.end(), it);
	*it = (uint8_t) ciudad.length();
	++it;
	it = std::copy(ciudad.begin(), ciudad.end(), it);
	uint32_t fechaBytes = fechaNacimiento.toUint32_t();
	it = std::copy((char*)&fechaBytes, (char*)(&fechaBytes+1), it);
	std::vector<char> geolocBytes = geolocacion.toBytes();
	it = std::copy(geolocBytes.begin(), geolocBytes.end(), it);
	std::copy((char*)&fotoID, (char*)(&fotoID+1), it);
	return result;
}
