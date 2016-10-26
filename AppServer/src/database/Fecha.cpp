#include "../../include/database/Fecha.h"
#include <sstream>

Fecha::Fecha() {}

Fecha::Fecha(const Fecha &fecha) : fecha(fecha.fecha) {}

Fecha::Fecha(const std::string& fecha) : fecha(0) {
	//TODO: Format check
	if (fecha == "current")	{
		this->fecha = 0xffffffff;
		return;
	}
	size_t barra1 = fecha.find('/');
	size_t barra2 = fecha.rfind('/');
	size_t lenDia = barra1;
	size_t lenMes = barra2 - barra1;
	size_t lenAnio = fecha.length() - barra2;
	this->fecha |= (std::stoi(fecha.substr(barra2+1, lenAnio)) << 16);
	this->fecha |= (std::stoi(fecha.substr(barra1+1,lenMes)) << 8);
	this->fecha |= std::stoi(fecha.substr(0,lenDia));
}

Fecha::Fecha(const char* byteArray) : fecha(0) {
	assign(byteArray);
}

void Fecha::assign(const char* byteArray) {
	std::copy(byteArray, byteArray+4, (char*)&fecha);
}

std::string Fecha::toString() const {
	if (fecha == 0xffffffff) return std::string("current");
	uint anio = (fecha & 0xffff0000) >> 16;
	uint mes = (fecha & 0x0000ff00) >> 8;
	uint dia = fecha & 0x000000ff;
	std::stringstream result;
	result << dia << "/" << mes << "/" << anio;
	return result.str();
}

uint32_t Fecha::toUint32_t() const {
	return fecha;
}

bool Fecha::operator< (const Fecha &fecha) {
	return this->fecha < fecha.fecha;
}
