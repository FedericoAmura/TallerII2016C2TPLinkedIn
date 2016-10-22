#include "../../include/database/Fecha.h"
#include <cstring>
#include <sstream>

Fecha::Fecha() {}

Fecha::Fecha(const Fecha &fecha) : fecha(fecha.fecha) {}

Fecha::Fecha(const std::string& fecha) : fecha(0) {
	//TODO: Format check
	this->fecha |= (std::stoi(fecha.substr(0,4)) << 16);
	this->fecha |= (std::stoi(fecha.substr(5,2)) << 8);
	this->fecha |= std::stoi(fecha.substr(8,2));
}

Fecha::Fecha(const char* byteArray) : fecha(0) {
	assign(byteArray);
}

void Fecha::assign(const char* byteArray) {
	std::memcpy(&fecha, byteArray, 4);
}

std::string Fecha::toString() {
	uint anio = (fecha & 0xffff0000) >> 16;
	uint mes = (fecha & 0x0000ff00) >> 8;
	uint dia = fecha & 0x000000ff;
	std::stringstream result;
	result << anio << "/" << mes << "/" << dia;
	return result.str();
}

uint32_t Fecha::toUint32_t() {
	return fecha;
}
